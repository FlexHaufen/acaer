/**
 * @file SceneSerializer.cpp
 * @author flexhaufen
 * @brief Scene Serializer
 * @version 0.1
 * @date 2023-02-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***ad
#include "acaer/Scene/SceneSerializer.h"
#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/Components.h"



// *** DEFINE ***

// *** NAMESPACE ***
namespace YAML {

    /**
     * @brief   Custom definiton for saving a v2f
     * 
     * @tparam  N/A
     */
	template<>
	struct convert<v2f> {
		static Node encode(const v2f rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, v2f& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<f32>(), 
            rhs.y = node[1].as<f32>();
			return true;
		}
	};

    /**
     * @brief   Custom definiton for saving a vColor
     * 
     * @tparam  N/A
     */
	template<>
	struct convert<vColor> {
		static Node encode(const vColor rhs) {
			Node node;
			node.push_back(rhs.r);
			node.push_back(rhs.g);
            node.push_back(rhs.b);
			node.push_back(rhs.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, vColor& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.r = node[0].as<u8>(); 
            rhs.g = node[1].as<u8>(); 
            rhs.b = node[2].as<u8>(); 
            rhs.a = node[3].as<u8>();
			return true;
		}
	};

    /**
     * @brief   Custom definiton for saving a UUID struct
     * 
     * @tparam  N/A
     */
    template<>
	struct convert<Acaer::UUID> {
		static Node encode(const Acaer::UUID& uuid) {
			Node node;
			node.push_back((u64)uuid);
			return node;
		}

		static bool decode(const Node& node, Acaer::UUID& uuid) {
			uuid = node.as<u64>();
			return true;
		}
	};

};

namespace Acaer {


    /**
     * @brief Overwrite operator for saving a v2f
     * 
     * @param out       YAML Emitter
     * @param v         v2f
     * @return YAML::Emitter& 
     */
    YAML::Emitter& operator<<(YAML::Emitter& out, const v2f& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

    /**
     * @brief Overwrite operator for saving a vColor
     * 
     * @param out       YAML Emitter
     * @param v         vColor
     * @return YAML::Emitter& 
     */
    YAML::Emitter& operator<<(YAML::Emitter& out, const vColor& v) {
		out << YAML::Flow;
        // NOTE: cast to u16 is necessary, else the value will be stored as a string
		out << YAML::BeginSeq << (u16)v.r << (u16)v.g << (u16)v.b << (u16)v.a << YAML::EndSeq;
		return out;
	}

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) 
    : m_Scene(scene){}

    void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity) {
    
        // TODO: assert here if entity has no tag

        out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();


		if (entity.HasComponent<Component::Tag>()) {
			auto& tag = entity.GetComponent<Component::Tag>().tag;
			out << YAML::Key << "Tag";
			out << YAML::BeginMap;
			out << YAML::Key << "tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

        if (entity.HasComponent<Component::Transform>()) {
            auto &c = entity.GetComponent<Component::Transform>();
            out << YAML::Key << "Transform";
            out << YAML::BeginMap;
            out << YAML::Key << "render_layer"  << YAML::Value << c.render_layer;
            out << YAML::Key << "pos"           << YAML::Value << c.pos;
            out << YAML::Key << "scale"         << YAML::Value << c.scale;
            out << YAML::Key << "rotation"      << YAML::Value << c.rotation;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Component::RigidBody>()) {
            auto &c = entity.GetComponent<Component::RigidBody>();
            out << YAML::Key << "RigitBody_C";
            out << YAML::BeginMap;
            out << YAML::Key << "type"                  << YAML::Value << (int)c.type;
            out << YAML::Key << "fixedRoation"          << YAML::Value << c.fixedRoation;
            out << YAML::Key << "density"               << YAML::Value << c.density;
            out << YAML::Key << "friction"              << YAML::Value << c.friction;
            out << YAML::Key << "restitution"           << YAML::Value << c.restitution;
            out << YAML::Key << "restitutionThreshold"  << YAML::Value << c.restitutionThreshold;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Component::Collider>()) {
            auto &c = entity.GetComponent<Component::Collider>();
            out << YAML::Key << "Collider";
            out << YAML::BeginMap;
            out << YAML::Key << "offset"        << YAML::Value << c.offset;
            out << YAML::Key << "size"          << YAML::Value << c.size;
            out << YAML::Key << "Sensors"       << YAML::BeginSeq;
            for (auto i : c.sensors) {
                out << YAML::BeginMap;
                out << YAML::Key << "id"            << YAML::Value << i.id;
                out << YAML::Key << "offset"        << YAML::Value << i.offset;
                out << YAML::Key << "size"          << YAML::Value << i.size;
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Component::Camera>()) {
            auto &c = entity.GetComponent<Component::Camera>();
            out << YAML::Key << "Camera";
            out << YAML::BeginMap;
            out << YAML::Key << "zoom"         << YAML::Value << c.zoom;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Component::Input>()) {
            auto &c = entity.GetComponent<Component::Input>();
            out << YAML::Key << "Input";
            out << YAML::BeginMap;
            out << YAML::Key << "isControllable"<< YAML::Value << c.isControllable;
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    }

    void SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& entity) {
        
        
        u64 uuid = entity["Entity"].as<u64>();
        std::string tag = entity["Tag"]["tag"].as<std::string>();

        Entity currentEntity = m_Scene->CreateEntityWithUUID(uuid, tag);
     
        auto transform_c = entity["Transform"];
        if (transform_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Component::Transform>();
            c.render_layer  = transform_c["render_layer"].as<s8>();
            c.pos           = transform_c["pos"].as<v2f>();
            c.scale         = transform_c["scale"].as<v2f>();
            c.rotation      = transform_c["rotation"].as<f32>();
        }

        auto rigitbody_c = entity["RigidBody"];
        if (rigitbody_c) {
            auto &c = currentEntity.GetOrEmplaceComponent<Component::RigidBody>();
            c.type                  = (Component::RigidBody::BodyType)rigitbody_c["type"].as<s8>();
            c.fixedRoation          = rigitbody_c["fixedRoation"].as<b8>();
            c.density               = rigitbody_c["density"].as<f32>();
            c.friction              = rigitbody_c["friction"].as<f32>();
            c.restitution           = rigitbody_c["restitution"].as<f32>();
            c.restitutionThreshold  = rigitbody_c["restitutionThreshold"].as<f32>();
        }

        auto collider_c = entity["Collider"];
        if (collider_c) {
            auto &c = currentEntity.GetOrEmplaceComponent<Component::Collider>();
            c.offset    = collider_c["offset"].as<v2f>();
            c.size      = collider_c["size"].as<v2f>();
            for (auto sensor : collider_c["Sensors"]) {
                Component::Collider::Sensor s;
                s.id        = sensor["id"].as<std::string>();
                s.offset    = sensor["offset"].as<v2f>();
                s.size      = sensor["size"].as<v2f>();
                c.sensors.push_back(s);
            }
        }

        auto camera_c = entity["Camera"];
        if (camera_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Component::Camera>();
            c.zoom = camera_c["zoom"].as<f32>();
        }

        auto input_c = entity["Input"];
        if (input_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Component::Input>();
            c.isControllable = input_c["isControllable"].as<b8>();
        }
    }

    void SceneSerializer::Serialize(const std::string& filepath) {
        
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key    << "Scene"      << YAML::Value  << "Untitled";        // TODO add scene name
        
        if (!m_Scene->m_Registry.empty()) {
            out << YAML::Key    << "Entities"   << YAML::Value  << YAML::BeginSeq;
            m_Scene->m_Registry.each([&](auto entityID) {
                Entity entity = {entityID, m_Scene.get()};
                if (!entity) {
                    return;
                }
                SerializeEntity(out, entity);
            });

            out << YAML::EndSeq;
        }
        
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath) {

    }


    b8 SceneSerializer::Deserialize(const std::string& filepath) {

        
        YAML::Node data;
		try {
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e) {
			std::cout << "Failed to load .acs file" << filepath << "\n" <<  e.what() << "\n";
			return false;
		}

        if(!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();

        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                DeserializeEntity(entity);
            }
        }
        return true;
    }

    b8 SceneSerializer::DeserializeRuntime(const std::string& filepath) {
        return false;
    }
}