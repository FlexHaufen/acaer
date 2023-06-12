/**
 * @file ScriptableEntity
 * @author flexhaufen
 * @brief Scriptclass for entity
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Entity.h"

namespace Acaer {

	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(f32 dt) {}
	private:

		// ** Members **
		Entity m_Entity;
		friend class Scene;
	};
}