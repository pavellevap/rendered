#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>

#include "Renderer.h"
#include "Primitives/Triangle.h"
#include "Primitives/Sphere.h"
#include "Primitives/Quadrangle.h"
#include "Primitives/Parallelepiped.h"
#include "ObjectArithmetic/Union.h"
#include "ObjectArithmetic/Complement.h"
#include "ObjectArithmetic/Difference.h"

using namespace std;

const size_t WIDTH = 800;
const size_t HEIGHT = 600;

SDL_Window* Window = nullptr;
SDL_Renderer* SDLRenderer = nullptr;

bool InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "SDL_Init error\n" << SDL_GetError() << endl;
        return false;
    }

    Window = SDL_CreateWindow("Hello World\n", 0, 0, 0 + WIDTH, 0 + HEIGHT, SDL_WINDOW_SHOWN);
    if (Window == nullptr) {
        cerr << "SDL_CreateWindow error\n" << SDL_GetError() << endl;
        return false;
    }

    SDLRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (SDLRenderer == nullptr){
        cerr << "SDL_CreateRenderer Error\n" << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void DrawImage(const RGBImage& image) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            ((unsigned int*)(surface->pixels))[i * WIDTH + j] = ((unsigned int)(image.getPixel(i, j).r * 255) << 24) +
                                                                ((unsigned int)(image.getPixel(i, j).g * 255) << 16) +
                                                                ((unsigned int)(image.getPixel(i, j).b * 255) << 8) +
                                                                255;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(SDLRenderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr){
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_RenderClear(SDLRenderer);
    SDL_RenderCopy(SDLRenderer, texture, nullptr, nullptr);
    SDL_RenderPresent(SDLRenderer);

    SDL_DestroyTexture(texture);
}

int MainLoop() {
    SDL_Event event;
    while (true)
        if (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                break;

    SDL_DestroyRenderer(SDLRenderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();

    return 0;
}

RGBImage* CreateChessboardTexture(size_t width, size_t height) {
    auto* texture = new RGBImage(width, height);
    for (size_t i = 0; i < height; i++)
        for (size_t j = 0; j < width; j++)
            if ((i + j) & 1)
                texture->setPixel(i, j, RGBColor::WHITE);
            else
                texture->setPixel(i, j, RGBColor::BLACK);
    return texture;
}

RGBImage* LoadTextureFromBMP(const std::string& filename) {
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    if (surface == nullptr) {
        std::cerr << "Cannot load texture. SDL error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    size_t bytesPerPixel = surface->format->BytesPerPixel;

    auto* texture = new RGBImage(surface->w, surface->h);
    for (size_t i = 0; i < surface->h; i++)
        for (size_t j = 0; j < surface->w; j++) { // mud
            size_t index = i * surface->w + j;
            double b = ((unsigned char*)(surface->pixels))[index * bytesPerPixel + 0] / 255.0;
            double g = ((unsigned char*)(surface->pixels))[index * bytesPerPixel + 1] / 255.0;
            double r = ((unsigned char*)(surface->pixels))[index * bytesPerPixel + 2] / 255.0;
            texture->setPixel(i, j, RGBColor(r, g, b));
        }

    return texture;
}

// Format support isn't complete.
// All lines must begin only with the following strings:
//    v, vt, vn, g, s, f
// After v must be exactly 3 doubles
// After vt must be exactly 3 doubles but the last one is ignored
// Symbol g is ignored
// Symbol s is ignored
// After f must be either 3 or 4 strings in following format: %d/%d/%d
Object* LoadObjectFromObj(const std::string& filename) {
    ifstream in(filename);

    std::vector<Point3D> vertices;
    std::vector<Point2D> texCoords;
    std::vector<Vector3D> norms;
    std::vector<Primitive*> primitives;

    std::string s;
    while (in >> s) {
        if (s.length() == 0 || s[0] == '#' || s[0] == 's' || s[0] == 'g') {
            std::getline(in, s);
            continue;
        }
        else if (s == "v") {
            Point3D vertex;
            in >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (s == "vn") {
            Vector3D norm;
            in >> norm.x >> norm.y >> norm.z;
            norms.push_back(norm);
        }
        else if (s == "vt") {
            Point2D coords;
            double tmp;
            in >> coords.x >> coords.y >> tmp;
            coords.x = coords.x - floorf(coords.x);
            coords.y = coords.y - floorf(coords.y);
            coords.y = 1 - coords.y;
            texCoords.push_back(coords);
        }
        else if (s == "f") {
            std::string line;
            std::getline(in, line);

            std::istringstream iss(line);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word)
                words.push_back(word);

            if (words.size() == 3) {
                auto* triangle = new Triangle();
                for (int i = 0; i < 3; i++) {
                    int v, vt, vn;
                    sscanf(words[i].c_str(), "%d/%d/%d", &v, &vt, &vn);
                    triangle->norms[i] = norms[vn - 1];
                    triangle->points[i] = vertices[v - 1];
                    triangle->texCoords[i] = texCoords[vt - 1];
                }
                triangle->material = nullptr;
                triangle->texture = nullptr;
                primitives.push_back(triangle);
            }
            else if (words.size() == 4) {
                auto* quadrangle = new Quadrangle();
                for (int i = 0; i < 4; i++) {
                    int v, vt, vn;
                    sscanf(words[i].c_str(), "%d/%d/%d", &v, &vt, &vn);
                    quadrangle->norms[i] = norms[vn - 1];
                    quadrangle->points[i] = vertices[v - 1];
                    quadrangle->texCoords[i] = texCoords[vt - 1];
                }
                quadrangle->material = nullptr;
                quadrangle->texture = nullptr;
                primitives.push_back(quadrangle);
            }
            else {
                cout << "error: " << words.size() << endl;
            }
        } else {
            cout << "error: " << s << ' ';
            std::getline(in, s);
            cout << s << endl;
            continue;
        }
    }

    return new Object(primitives);
}

Scene LoadScene1(size_t width, size_t height) {
    Scene scene;

    RGBImage* waterTexture = LoadTextureFromBMP("textures/Water.bmp");

    auto* quadrangle = new Quadrangle;
    quadrangle->points[0] = Point3D(-1.0f, -1.0f, -1.5f);
    quadrangle->points[1] = Point3D(-1.0f, 1.0f, -1.5f);
    quadrangle->points[2] = Point3D(1.0f, 1.0f, -1.5f);
    quadrangle->points[3] = Point3D(1.0f, -1.0f, -1.5f);

    quadrangle->norms[0] = Vector3D(0, 0, 1);
    quadrangle->norms[1] = Vector3D(0, 0, 1);
    quadrangle->norms[2] = Vector3D(0, 0, 1);
    quadrangle->norms[3] = Vector3D(0, 0, 1);

    quadrangle->texCoords[0] = Point2D(0, 1);
    quadrangle->texCoords[1] = Point2D(0, 0);
    quadrangle->texCoords[2] = Point2D(1, 0);
    quadrangle->texCoords[3] = Point2D(1, 1);

    quadrangle->material = (Material::BLACK_HOLE);
    quadrangle->texture = (waterTexture);

    auto* rectangle = new Object({quadrangle});

    auto* sphere = new Sphere;
    sphere->center = Point3D(0, -0.2f, -1);
    sphere->radius = 0.1;
    sphere->setMaterial(Material::changeColor(Material::BLACK_HOLE, RGBColor::BLUE));

    auto* sphere1 = new Sphere;
    sphere1->center = Point3D(0, 0, -1);
    sphere1->radius = 0.2;
    sphere1->setMaterial(Material::changeColor(Material::BLACK_HOLE, RGBColor::RED));

    auto* sphere2 = new Sphere;
    sphere2->center = Point3D(sinf(104 / 3.1415f) * 0.2f, -cosf(104 / 3.1415f) * 0.2f, -1);
    sphere2->radius = 0.1;
    sphere2->setMaterial(Material::changeColor(Material::BLACK_HOLE, RGBColor::BLUE));

    auto* molecule = new Object({sphere, sphere1, sphere2});

    auto* source1 = new PointLight;
    source1->position = Point3D(2, 0, 1);
    source1->color = RGBColor::WHITE;
    source1->quadricAttenuation = 0.2f;

    auto* source2 = new PointLight;
    source2->position = Point3D(1, 1, 1);
    source2->color = RGBColor::WHITE;
    source2->quadricAttenuation = 0.3f;

    Camera camera(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    auto* ambientLight = new AmbientLight;
    ambientLight->color = RGBColor(0.2, 0.2, 0.2);

    scene.camera = camera;
    scene.things.push_back(rectangle);
    scene.things.push_back(molecule);
    scene.pointLights.push_back(source1);
    scene.pointLights.push_back(source2);
    scene.ambientLight = ambientLight;

    return scene;
}

Scene LoadScene2(size_t width, size_t height) {
    Scene scene;

    Object* gnom = LoadObjectFromObj("objects/Gnom/Gnom.obj");

    RGBImage* texture = LoadTextureFromBMP("objects/Gnom/gnom.bmp");

    auto* source1 = new PointLight;
    source1->position = Point3D(10, 5, 10);
    source1->color = RGBColor::WHITE;
    source1->quadricAttenuation = 1.0f / 150;

    auto* source2 = new AmbientLight;
    source2->color = RGBColor(0.5f, 0.5f, 0.5f);

    scene.pointLights.push_back(source1);
    scene.ambientLight = source2;

    gnom->setTexture(texture);

    Camera camera(Point3D(0, 0, 20), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    scene.camera = camera;
    scene.things.push_back(gnom);

    return scene;
}

Scene LoadScene3(size_t width, size_t height) {
    Scene scene;

    auto* quadrangle = new Quadrangle;
    quadrangle->points[0] = Point3D(-2.0f, -0.05f, -2.0f);
    quadrangle->points[1] = Point3D(-2.0f, -0.05f, 2.0f);
    quadrangle->points[2] = Point3D(2.0f, -0.05f, 2.0f);
    quadrangle->points[3] = Point3D(2.0f, -0.05f, -2.0f);

    quadrangle->texCoords[0] = Point2D(0, 0);
    quadrangle->texCoords[1] = Point2D(0, 1);
    quadrangle->texCoords[2] = Point2D(1, 1);
    quadrangle->texCoords[3] = Point2D(1, 0);

    quadrangle->norms[0] = Vector3D(0, 1, 0);
    quadrangle->norms[1] = Vector3D(0, 1, 0);
    quadrangle->norms[2] = Vector3D(0, 1, 0);
    quadrangle->norms[3] = Vector3D(0, 1, 0);

    quadrangle->texture = CreateChessboardTexture(100, 100);
    quadrangle->material = Material::POLISHED_METAL;

    auto* quadrangle2 = new Quadrangle;
    quadrangle2->points[0] = Point3D(-2.0f, 1.0f, -2.0f);
    quadrangle2->points[1] = Point3D(-2.0f, 1.0f, 2.0f);
    quadrangle2->points[2] = Point3D(2.0f, 1.0f, 2.0f);
    quadrangle2->points[3] = Point3D(2.0f, 1.0f, -2.0f);

    quadrangle2->texCoords[0] = Point2D(0, 0);
    quadrangle2->texCoords[1] = Point2D(0, 1);
    quadrangle2->texCoords[2] = Point2D(1, 1);
    quadrangle2->texCoords[3] = Point2D(1, 0);

    quadrangle2->norms[0] = Vector3D(0, -1, 0);
    quadrangle2->norms[1] = Vector3D(0, -1, 0);
    quadrangle2->norms[2] = Vector3D(0, -1, 0);
    quadrangle2->norms[3] = Vector3D(0, -1, 0);

    quadrangle2->texture = CreateChessboardTexture(100, 100);
    quadrangle2->material = Material::POLISHED_METAL;

    auto* rectangle = new Object({quadrangle/*, quadrangle2*/});

    auto* sphere1 = new Sphere;
    sphere1->center = Point3D(0.25f, 0.25f, 1);
    sphere1->radius = 0.25;

    sphere1->material = Material::changeColor(Material::POLISHED_METAL, RGBColor::YELLOW);

    auto* sphere2 = new Sphere;
    sphere2->center = Point3D(-0.25f, 0.25f, 1);
    sphere2->radius = 0.25;

    sphere2->material = Material::changeColor(Material::POLISHED_METAL, RGBColor::RED);

    auto* sphere3 = new Sphere;
    sphere3->center = Point3D(0, 0.25f + 2 * 0.25f * std::sin(3.1415f / 3), 1);
    sphere3->radius = 0.25;

    sphere3->material = Material::changeColor(Material::POLISHED_METAL, RGBColor::GREEN);

    auto* source1 = new PointLight;
    source1->position = Point3D(1.0, 0.5f, 1);
    source1->color = RGBColor(0.99f, 0.99f, 0.6f);
    source1->quadricAttenuation = 0;

    auto* source2 = new AmbientLight;
    source2->color = RGBColor(0.3f, 0.3f, 0.3f);

    Camera camera(Point3D(0, 0, -1), Vector3D(0, 0, 1), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    scene.camera = camera;
    scene.things.push_back(rectangle);
    scene.things.push_back(new Object({sphere1, sphere2, sphere3}));
    scene.pointLights.push_back(source1);
    scene.ambientLight = source2;

    return scene;
}

Scene LoadScene4(size_t width, size_t height) {
    Scene scene;

    auto* quadrangle = new Quadrangle;
    quadrangle->points[0] = Point3D(-2.0f, -0.1f, -2.0f);
    quadrangle->points[1] = Point3D(-2.0f, -0.1f, 2.0f);
    quadrangle->points[2] = Point3D(2.0f, -0.1f, 2.0f);
    quadrangle->points[3] = Point3D(2.0f, -0.1f, -2.0f);

    quadrangle->texCoords[0] = Point2D(0, 0);
    quadrangle->texCoords[1] = Point2D(0, 1);
    quadrangle->texCoords[2] = Point2D(1, 1);
    quadrangle->texCoords[3] = Point2D(1, 0);

    quadrangle->norms[0] = Vector3D(0, 1, 0);
    quadrangle->norms[1] = Vector3D(0, 1, 0);
    quadrangle->norms[2] = Vector3D(0, 1, 0);
    quadrangle->norms[3] = Vector3D(0, 1, 0);

    quadrangle->texture = CreateChessboardTexture(100, 100);
    quadrangle->material = Material::METAL;

    auto* quadrangle2 = new Quadrangle;
    quadrangle2->points[0] = Point3D(-0.3f, -2.0f, 2.0f);
    quadrangle2->points[1] = Point3D(-0.3f, -2.0f, -2.0f);
    quadrangle2->points[2] = Point3D(-0.3f, 2.0f, -2.0f);
    quadrangle2->points[3] = Point3D(-0.3f, 2.0f, 2.0f);

    quadrangle2->texCoords[0] = Point2D(0, 0);
    quadrangle2->texCoords[1] = Point2D(0, 1);
    quadrangle2->texCoords[2] = Point2D(1, 1);
    quadrangle2->texCoords[3] = Point2D(1, 0);

    quadrangle2->norms[0] = Vector3D(-1, 0, 0);
    quadrangle2->norms[1] = Vector3D(-1, 0, 0);
    quadrangle2->norms[2] = Vector3D(-1, 0, 0);
    quadrangle2->norms[3] = Vector3D(-1, 0, 0);

    quadrangle2->texture = nullptr;
    quadrangle2->material = Material::changeColor(Material::WATER, RGBColor::BLUE);

    auto* quadrangle3 = new Quadrangle;
    quadrangle3->points[0] = Point3D(0.5f, -2.0f, 2.0f);
    quadrangle3->points[1] = Point3D(0.5f, -2.0f, -2.0f);
    quadrangle3->points[2] = Point3D(0.5f, 2.0f, -2.0f);
    quadrangle3->points[3] = Point3D(0.5f, 2.0f, 2.0f);

    quadrangle3->texCoords[0] = Point2D(0, 0);
    quadrangle3->texCoords[1] = Point2D(0, 1);
    quadrangle3->texCoords[2] = Point2D(1, 1);
    quadrangle3->texCoords[3] = Point2D(1, 0);

    quadrangle3->norms[0] = Vector3D(-1, 0, 0);
    quadrangle3->norms[1] = Vector3D(-1, 0, 0);
    quadrangle3->norms[2] = Vector3D(-1, 0, 0);
    quadrangle3->norms[3] = Vector3D(-1, 0, 0);

    quadrangle3->texture = nullptr;
    quadrangle3->material = Material::changeColor(Material::otherSide(Material::WATER), RGBColor::BLUE);

    auto* parallelepiped = new Parallelepiped;
    parallelepiped->vertex = Point3D(-1, -0.1f, 0.5f);
    parallelepiped->edges[0] = Vector3D(2.0f, 0, -1.0f);
    parallelepiped->edges[1] = Vector3D(0, 0.1f, 0);
    parallelepiped->edges[2] = Vector3D(-0.1f, 0, -0.2f) / std::sqrt(0.05) * 0.1;
    parallelepiped->setMaterial(Material::changeColor(Material::METAL, RGBColor::RED));

    auto* source1 = new PointLight;
    source1->color = RGBColor(1.0f, 1.0f, 1.0f);
    source1->position = Point3D(-0.5f, 0.2f, -0.7f);
    source1->quadricAttenuation = 0.001f;
    auto* source2 = new AmbientLight;
    source2->color = RGBColor(0.1f, 0.1f, 0.1f);

    Camera camera(Point3D(-1.0, 0.25f, 0), Vector3D(1.0, -0.25f, 0), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    scene.camera = camera;
    scene.things.push_back(new Object({parallelepiped}));
    scene.things.push_back(new Object({quadrangle}));
    scene.things.push_back(new Object({quadrangle2}));
    scene.things.push_back(new Object({quadrangle3}));
    scene.ambientLight = source2;
    scene.pointLights.push_back(source1);

    return scene;
}

Scene LoadScene5(size_t width, size_t height) {
    Scene scene;

    RGBImage* texture = LoadTextureFromBMP("textures/arrow.bmp");

    auto* quadrangle = new Quadrangle;
    quadrangle->points[0] = Point3D(-2.0f, -0.1f, -2.0f);
    quadrangle->points[1] = Point3D(-2.0f, -0.1f, 2.0f);
    quadrangle->points[2] = Point3D(2.0f, -0.1f, 2.0f);
    quadrangle->points[3] = Point3D(2.0f, -0.1f, -2.0f);

    quadrangle->texCoords[0] = Point2D(0, 0);
    quadrangle->texCoords[1] = Point2D(0, 1);
    quadrangle->texCoords[2] = Point2D(1, 1);
    quadrangle->texCoords[3] = Point2D(1, 0);

    quadrangle->norms[0] = Vector3D(0, 1, 0);
    quadrangle->norms[1] = Vector3D(0, 1, 0);
    quadrangle->norms[2] = Vector3D(0, 1, 0);
    quadrangle->norms[3] = Vector3D(0, 1, 0);

    quadrangle->texture = CreateChessboardTexture(100, 100);
    quadrangle->material = Material::METAL;

    auto* sphere = new Sphere;
    sphere->center = Point3D(0, 0, 0);
    sphere->radius = 0.1;
    sphere->material = Material::WATER;

    auto* sphere2 = new Sphere;
    sphere2->center = Point3D(0, 0, 0);
    sphere2->radius = 0.099;
    sphere2->material = Material::otherSide(Material::WATER);

    auto* parallelepiped = new Parallelepiped;
    parallelepiped->vertex = Point3D(0.4f, -0.1f, -0.5f);
    parallelepiped->edges[0] = Vector3D(0.0f, 0.0f, 1.0f);
    parallelepiped->edges[1] = Vector3D(0.0f, 0.1f, 0.0f);
    parallelepiped->edges[2] = Vector3D(0.1f, 0.0f, 0.0f);

    parallelepiped->setTexture(texture);
    parallelepiped->setMaterial(Material::changeColor(Material::METAL, RGBColor::RED));

    auto* source1 = new PointLight;
    source1->color = RGBColor(1.0f, 1.0f, 1.0f);
    source1->position = Point3D(-0.5f, 0.2f, -0.7f);
    source1->quadricAttenuation = 0.001f;
    auto* source2 = new AmbientLight;
    source2->color = RGBColor(0.1f, 0.1f, 0.1f);

    Camera camera(Point3D(-1.0f, 0.25f, 0), Vector3D(1.0, -0.25f, 0), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    scene.camera = camera;
    scene.things.push_back(new Object({parallelepiped}));
    scene.things.push_back(new Object({quadrangle}));
    scene.things.push_back(new Object({sphere}));
    scene.things.push_back(new Complement(sphere2));
    scene.ambientLight = source2;
    scene.pointLights.push_back(source1);

    return scene;
}

Scene LoadScene6(size_t width, size_t height) {
    Scene scene;

    RGBImage* texture = LoadTextureFromBMP("textures/EarthMap.bmp");

    auto* quadrangle = new Quadrangle;
    quadrangle->points[0] = Point3D(-2.0f, -0.1f, -2.0f);
    quadrangle->points[1] = Point3D(-2.0f, -0.1f, 2.0f);
    quadrangle->points[2] = Point3D(2.0f, -0.1f, 2.0f);
    quadrangle->points[3] = Point3D(2.0f, -0.1f, -2.0f);

    quadrangle->texCoords[0] = Point2D(0, 0);
    quadrangle->texCoords[1] = Point2D(0, 1);
    quadrangle->texCoords[2] = Point2D(1, 1);
    quadrangle->texCoords[3] = Point2D(1, 0);

    quadrangle->norms[0] = Vector3D(0, 1, 0);
    quadrangle->norms[1] = Vector3D(0, 1, 0);
    quadrangle->norms[2] = Vector3D(0, 1, 0);
    quadrangle->norms[3] = Vector3D(0, 1, 0);

    quadrangle->texture = CreateChessboardTexture(100, 100);
    quadrangle->material = Material::METAL;

    auto* sphere = new Sphere;
    sphere->center = Point3D(0, 0, 0);
    sphere->radius = 0.1;
    sphere->material = Material::METAL;
    sphere->texture = texture;

    auto* parallelepiped = new Parallelepiped;
    parallelepiped->vertex = Point3D(0.0f, 0.0f, -0.0f);
    parallelepiped->edges[0] = Vector3D(0.0f, 0.0f, 0.1f);
    parallelepiped->edges[1] = Vector3D(0.0f, 0.1f, 0.0f);
    parallelepiped->edges[2] = Vector3D(0.1f, 0.0f, 0.0f);
    parallelepiped->setMaterial(Material::changeColor(Material::METAL, RGBColor::RED));

    auto* smallSphere = new Sphere;
    smallSphere->center = Point3D(0.0f, 0.1f, 0.0f);
    smallSphere->radius = 0.03;
    smallSphere->material = Material::changeColor(Material::METAL, RGBColor::YELLOW);

    auto* sphere2 = new Sphere;
    sphere2->center = Point3D(0.1f, 0.1f, 0.1f);
    sphere2->radius = 0.1;
    sphere2->material = Material::METAL;
    sphere2->texture = texture;

    auto* parallelepiped2 = new Parallelepiped;
    parallelepiped2->vertex = Point3D(0.1f, 0.1f, 0.1f);
    parallelepiped2->edges[0] = Vector3D(0.0f, 0.0f, 0.1f);
    parallelepiped2->edges[1] = Vector3D(0.0f, 0.1f, 0.0f);
    parallelepiped2->edges[2] = Vector3D(0.1f, 0.0f, 0.0f);
    parallelepiped2->setMaterial(Material::changeColor(Material::METAL, RGBColor::RED));

    auto* smallSphere2 = new Sphere;
    smallSphere2->center = Point3D(0.1f, 0.1f, 0.2f);
    smallSphere2->radius = 0.03;
    smallSphere2->material = Material::changeColor(Material::METAL, RGBColor::YELLOW);

    auto* source1 = new PointLight;
    source1->color = RGBColor(1.0f, 1.0f, 1.0f);
    source1->position = Point3D(0.00f, 0.4f, 0.00f);
    source1->quadricAttenuation = 0.0f;
    auto* source3 = new PointLight;
    source3->color = RGBColor(1.0f, 1.0f, 1.0f);
    source3->position = Point3D(-0.5f, 0.7f, 0.0f);
    source3->quadricAttenuation = 0.0f;
    auto* source2 = new AmbientLight;
    source2->color = RGBColor(0.1f, 0.1f, 0.1f);

    Camera camera(Point3D(-0.1f, 0.4f, 0.5f), Vector3D(0.1f, -0.4f, -0.5f), Vector3D(0, 1, 0), 1.0f, 1.0f * width / height, 1.0f);

    scene.camera = camera;
    scene.things.push_back(new Object({quadrangle}));
    scene.things.push_back(new Union(new Intersection(sphere2, parallelepiped2), smallSphere2));
    scene.things.push_back(new Difference(new Difference(sphere, parallelepiped), smallSphere));
    scene.ambientLight = source2;
    scene.pointLights.push_back(source1);
    scene.pointLights.push_back(source3);

    return scene;
}

int main() {

    if (!InitializeSDL()) {
        cerr << "Failed to initialize SDL\n";
        return 1;
    }

    Scene scene = LoadScene6(WIDTH, HEIGHT);
    Renderer rend(WIDTH, HEIGHT, 3);

    auto start = std::chrono::steady_clock::now();
    RGBImage image = rend.renderScene(scene);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "render time: " << elapsed_ms.count() << " ms\n";

    DrawImage(image);

    return MainLoop();
}