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

  
    // *** Input ***
    struct Input_c  {
    };


    struct Camera_c {
    };

};
```


## EnTT
[source](https://skypjack.github.io/entt/md_docs_md_entity.html)

Add Component to entity

```cpp
    entt::registry Registry;                          // EnTT Registry
    entt::entity my_entt = Registry.create();         // Create new EnTT


    Registry.emplace<Transform_Component>(my_entt);   // Add Transform component to EnTT

```

Iterate throug all entities with certain components
```cpp

    // Itterate throug all entitys with Transform Component
    auto view = Registry.view<Transform_Component>();
    for (auto entity : view) {
        auto &transform = view.get<Transform_Component>(entity);
    }


    // Itterate throug all entitys with group of components
    // Better than multi type views.
    auto group = Registry.group<Transform_Component, RigitBody_Component>();
    for (auto entity : group) {
        auto &transform = group.get<Transform_Component>(entity);
        auto &transform = group.get<RigitBody_Component>(entity);
    }
```