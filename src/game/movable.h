#pragma once
#include <SFML/System/Vector3.hpp>
namespace ss {
namespace game {

class Movable {
public:
    // ------------------------------------------------------------
    // construct / destruct
    // ------------------------------------------------------------
                Movable() = default;
    explicit    Movable(const sf::Vector3f initial_position);
    
    // ------------------------------------------------------------
    // methods
    // ------------------------------------------------------------
    inline void         setPosition (const sf::Vector3f pos);
    inline sf::Vector3f getPosition () const {return position;}
    void update(const float dt);
   
private:
    // ------------------------------------------------------------
    // members
    // ------------------------------------------------------------
    sf::Vector3f position;
    sf::Vector3f velocity;
    float mass = 1;
    float co_bounciness = -0.1F;
    float co_friction = 0.1f;
    float co_air_friction = 0.1F;
    float co_spin_decay = 0.1F;
    float cross_section = 1.0F;
    // TODO air density chould come from outside
    float air_density = 0.1F;
    
    struct {
        sf::Vector3f kinetic;
        sf::Vector3f rotation;
        
        void reset(){
            kinetic.x = kinetic.y = kinetic.z = 0;
            rotation.x = rotation.y = rotation.z = 0;
        }
    } forces;
    
    bool effected_by_gravity = false;
    
    // ------------------------------------------------------------
    // functions
    // ------------------------------------------------------------
    void            euler       (float dt);
    sf::Vector3f    integrate   (const float dt);
};

} // namespace game
} // namespace ss
