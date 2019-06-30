#include "editor.hpp"

#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#define ROOT "../"
#define FILE "res/maps/test.json"

#include <vector>
#include <string>

Editor::Editor() 
    : m_tile_scale(1.f)
    , m_camera_x(0)
    , m_camera_y(0)
    , logger() {}

Editor::~Editor() {}

void Editor::init() {
    logger.init();
    // test_animatedTexture = new AnimatedTexture("res/animate.png");
    // test_animatedTexture->generateAtlas(64, 80);
    // test_animatedTexture->addAnimationState(0, 1);
    // test_animatedTexture->changeAnimation(0);
    // test_track = loadWAV("res/test.wav", false);
    // createFont("test", "res/Munro.ttf", 20);
    std::string path(ROOT);
    path += FILE;

    std::ifstream file(path);
    if (file.is_open()) {
        json data;
        file >> data;

        json tilesheet = data["tilesheet"];
        m_tilesheet_src = tilesheet["source"];
        m_tile_size = tilesheet["tile_size"];
        m_tilesheet_width = tilesheet["width"];
        m_tilesheet_height = tilesheet["height"];
        
        m_map_width = data["width"];
        m_map_height = data["height"];

        // Can't directly copy to member for some reason?
        std::vector<int> temp = data["data"];
        m_tilemap = std::move(temp);
    } else {
        // TODO: Error logging
    }
}

void Editor::cleanup() {
    // TODO: Move deserialization to a 'save' button
    std::string path(ROOT);
    path += FILE;

    std::ofstream file(path);
    if (file.is_open()) {
        json data;
        
        json tilesheet;
        tilesheet["source"] = m_tilesheet_src;
        tilesheet["tile_size"] = m_tile_size;
        tilesheet["width"] = m_tilesheet_width;
        tilesheet["height"] = m_tilesheet_height;
        data["tilesheet"] = std::move(tilesheet);

        data["width"] = m_map_width;
        data["height"] = m_map_height;

        data["data"] = m_tilemap;
        file << data;
    } else {
        // TODO: Error logging
    }
}

void Editor::pause() {}
void Editor::resume() {}

void Editor::update() {
    if (keyPressed(SDL_SCANCODE_ESCAPE)) {
        exit();
    }
    if (getMouseScrollUp() > 0) {
        m_tile_scale += 0.1f * getMouseScrollUp();
    }
    if (getMouseScrollDown() > 0) {
        m_tile_scale -= 0.1f * getMouseScrollDown();
    }
    if (keyPressed(SDL_SCANCODE_SPACE)) {
        if (!m_panning) {
            m_panning = true;
            m_pan_start_mouse_x = getMouseX();
            m_pan_start_mouse_y = getMouseY();
            m_pan_start_camera_x = m_camera_x;
            m_pan_start_camera_y = m_camera_y;
        }
        m_camera_x = m_pan_start_camera_x + m_pan_start_mouse_x - getMouseX();
        m_camera_y = m_pan_start_camera_y + m_pan_start_mouse_y - getMouseY();
    } else {
        m_panning = false;
    }
}

void Editor::render() {
    // // Render animated textures
    // test_animatedTexture->render(0, 0);
    // // Render text
    // Texture testText(getTextTexture("test", "test", {200, 100, 100, 255}));
    // testText.render(200, 200);
    std::string texture_src(ROOT);
    texture_src += m_tilesheet_src;

    // TODO: Move this outside of the render function
    TileMap tiles(texture_src);
    tiles.generateTiles(m_tile_size, m_tile_size);

    for (int y = 0; y < m_map_height; ++y) {
        for (int x = 0; x < m_map_width; ++x) {
            // TODO: Better error handling
            unsigned int index = m_tilemap[y * m_map_width + x];
            tiles.render(
                static_cast<int>(std::ceil(static_cast<float>(x) * base_tile_size * m_tile_scale) - m_camera_x),
                static_cast<int>(std::ceil(static_cast<float>(y) * base_tile_size * m_tile_scale) - m_camera_y), 
                static_cast<int>(std::ceil(base_tile_size * m_tile_scale)), 
                static_cast<int>(std::ceil(base_tile_size * m_tile_scale)), 
                index
            );
        }
    }

    {   // Render the tile outline for the currently hovered over tile
        static Texture outline("resources/outline.png");
        int tile_x = static_cast<int>((m_camera_x + getMouseX()) / (base_tile_size * m_tile_scale));
        int tile_y = static_cast<int>((m_camera_y + getMouseY()) / (base_tile_size * m_tile_scale));
        outline.render(
            static_cast<int>(tile_x * base_tile_size * m_tile_scale - m_camera_x),
            static_cast<int>(tile_y * base_tile_size * m_tile_scale - m_camera_y),
            static_cast<int>(base_tile_size * m_tile_scale),
            static_cast<int>(base_tile_size * m_tile_scale)
        );
    }

    logger.render();
}