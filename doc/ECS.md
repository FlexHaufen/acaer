# ECS

## Components

```cpp
namespace Components {

    struct Sprite_c {
        Sprite sprite;
        Texture sprite_texture;        
    };


    struct Transform {
        
        std::string name;

        //v2 center_pos;
        v2<int> size;

        s8 render_layer;           // Order in the renderer

        // Debug hitbox
        RectangleShape hitbox;
    };


    // *** RIGITBODY ***
    struct RigidBody {
        f32 mass;                  // Mass of entity
        f32 inv_mass = 0.f;        // Inverse mass for easy calculations
        b8  isMovable;             // inf. mass

        f32 restitution;      
        f32 drag;                   // Decay rate of liniar velocity
        // f32 angularDrag;         // Rotational decay rate

        b8  usesGravity;            // true: affectet by gravity
        f32 gravityFactor;          // gravity multiplyer factor 

        b8  isPassible;


        // privat
        v2<float> velocity;

    };

    struct Collider {

    }

  
    // *** Player ***
    struct Player_c  {


    };


    struct Camera_c {



    };

};
```