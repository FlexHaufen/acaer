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

// *** INCLUDES ***
#include "acaer/Scene/SceneSerializer.h"
#include "acaer/Scene/Entity.h"
#include "acaer/Scene/Components.h"



// *** DEFINE ***

// *** NAMESPACE ***
namespace YAML {

    /**
     * @brief   Custom definiton for saving a Raylib Rectangle struct
     * 
     * @tparam  N/A
     */
	template<>
	struct convert<Rectangle> {
		static Node encode(const Rectangle& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.width);
			node.push_back(rhs.height);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Rectangle& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x       = node[0].as<f32>();
			rhs.y       = node[1].as<f32>();
            rhs.width   = node[2].as<f32>();
			rhs.height  = node[3].as<f32>();
			return true;
		}
	};

    /**
     * @brief   Custom definiton for saving a Raylib Color struct
     * 
     * @tparam  N/A
     */
	template<>
	struct convert<Color> {
		static Node encode(const Color& rhs) {
			Node node;
			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Color& rhs) {
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
     * @brief Overwrite operator for saving a Raylib Rectangle struct
     * 
     * @param out       YAML Emitter
     * @param r         Raylib Rectangle struct
     * @return YAML::Emitter& 
     */
    YAML::Emitter& operator<<(YAML::Emitter& out, const Rectangle& r) {
		out << YAML::Flow;
		out << YAML::BeginSeq << r.x << r.y << r.width << r.height << YAML::EndSeq;
		return out;
	}

    /**
     * @brief Overwrite operator for saving a Raylib Rectangle struct
     * 
     * @param out       YAML Emitter
     * @param c         Raylib Color struct
     * @return YAML::Emitter& 
     */
	YAML::Emitter& operator<<(YAML::Emitter& out, const Color& c) {
		out << YAML::Flow;
        // NOTE: The colors are saved as u16 instead of unsigned char's. Because char's will
        //       be saved as strings and thats shit
		out << YAML::BeginSeq << (u16)c.r << (u16)c.g << (u16)c.b << (u16)c.a << YAML::EndSeq;
		return out;
	}


    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) 
    : m_Scene(scene){}

    void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity) {
    
        // TODO: assert here if entity has no tag

        out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();


		if (entity.HasComponent<Tag_C>()) {
			auto& tag = entity.GetComponent<Tag_C>().tag;
			out << YAML::Key << "Tag_C";
			out << YAML::BeginMap;
			out << YAML::Key << "tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

        if (entity.HasComponent<Transform_C>()) {
            auto &c = entity.GetComponent<Transform_C>();
            out << YAML::Key << "Transform_C";
            out << YAML::BeginMap;
            out << YAML::Key << "render_layer"  << YAML::Value << c.render_layer;
            out << YAML::Key << "rec"           << YAML::Value << c.rec;
            out << YAML::Key << "color"         << YAML::Value << c.color;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Camera_C>()) {
            auto &c = entity.GetComponent<Camera_C>();
            out << YAML::Key << "Camera_C";
            out << YAML::BeginMap;
            out << YAML::Key << "zoom"         << YAML::Value << c.zoom;
            out << YAML::EndMap;
        }

        if (entity.HasComponent<Input_C>()) {
            auto &c = entity.GetComponent<Input_C>();
            out << YAML::Key << "Input_C";
            out << YAML::BeginMap;
            out << YAML::Key << "isControllable"<< YAML::Value << c.isControllable;
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    }

    void SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& entity) {
        
        
        u64 uuid = entity["Entity"].as<u64>();
        std::string tag = entity["Tag_C"]["tag"].as<std::string>();

        Entity currentEntity = m_Scene->CreateEntityWithUUID(uuid, tag);
     
        auto transform_c = entity["Transform_C"];
        if (transform_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Transform_C>();
            c.render_layer = transform_c["render_layer"].as<s8>();
            c.rec = transform_c["rec"].as<Rectangle>();
            c.color = transform_c["color"].as<Color>();
        }

        auto camera_c = entity["Camera_C"];
        if (camera_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Camera_C>();
            c.zoom = camera_c["zoom"].as<f32>();
        }

        auto input_c = entity["Input_C"];
        if (input_c) {
            auto& c = currentEntity.GetOrEmplaceComponent<Input_C>();
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