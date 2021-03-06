#pragma once
#include "core/engine.hpp"

#include <string>
#include "nlohmann/json.hpp"

class ArchetypeManager;
class Entity {

public:
    Entity(ArchetypeManager * archetypeManager);
    Entity(ArchetypeManager * archetypeManager, const std::string& name, const std::string& archetype);
    Entity(ArchetypeManager * archetypeManager, nlohmann::json data);
    Entity(Entity && rhs) noexcept;
    Entity& operator=(Entity& rhs);
    ~Entity();

    void render(int camera_x, int camera_y, float scale, bool outline = false);

    std::string get_name() const;
    std::string get_archetype() const;
    int get_x() const { return m_x; }
    int get_y() const { return m_y; }
    int get_w() const { return m_w; }
    int get_h() const { return m_h; }
    void set_pos(int x, int y) { m_x = x; m_y = y; }

    // Use this function for serizliating
    nlohmann::json get_entity_json();

    // Other utility functions
    void load_texture();
    bool has_texture() const;
    Texture * get_texture() const;
private:
    // Use the raw json to store data
    nlohmann::json m_data;
    // Other cool data
    int m_x, m_y;

    ArchetypeManager * archetypeManager;

    // Cache texture for EZ optimization
    bool m_has_texture;
    std::string m_texture;
    int m_w, m_h;
    // This is only relevant if animation exists
    bool m_animated;
    int m_target_x, m_target_y;
    int m_target_w, m_target_h;
};