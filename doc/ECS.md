# ECS

# EnTT
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