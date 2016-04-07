/**
**    Hatchit Engine
**    Copyright(c) 2015 Third-Degree
**
**    GNU Lesser General Public License
**    This file may be used under the terms of the GNU Lesser
**    General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.LGPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU Lesser General Public License requirements
**    will be met: https://www.gnu.org/licenses/lgpl.html
**
**/

#include <ht_gameobject.h>
#include <ht_debug.h>

#include <algorithm>

namespace Hatchit {

    namespace Game {
        GameObject::GameObject(void)
        {
            m_destroy = 0;
            m_parent = nullptr;
            m_components = std::vector<Component*>();
            m_children = std::vector<GameObject*>();
            m_componentMap = std::unordered_map<Core::Guid, std::vector<Component*>::size_type>();
        }

        GameObject::GameObject(const Core::Guid& guid, const std::string& name, Transform& t, bool enabled)
            : GameObject()
        {
            m_guid = guid;
            m_name = name;
            m_transform = t;
            m_enabled = enabled;
        }

        GameObject::~GameObject(void)
        {
            for (GameObject* child : m_children)
            {
                delete child;
            }
            for (Component *component: m_components)
            {
                if (component->m_enabled)
                    component->SetEnabled(false);
                delete component;
            }
        }

        const Core::Guid& GameObject::GetGuid(void) const
        {
            return m_guid;
        }

        const std::string& GameObject::GetName(void) const
        {
            return m_name;
        }

        Transform& GameObject::GetTransform(void)
        {
            return m_transform;
        }

        bool GameObject::GetEnabled(void) const
        {
            return m_enabled;
        }

        void GameObject::SetEnabled(bool value)
        {
            m_enabled = value;
        }

        GameObject* GameObject::GetParent(void)
        {
            return m_parent;
        }

        void GameObject::SetParent(GameObject *parent)
        {
            HT_DEBUG_PRINTF("GameObject SetParent. (not implemented)\n");
        }

        void GameObject::Update(void)
        {
            for (Component *component : m_components)
            {
                    if(component->GetEnabled())
                        component->VOnUpdate();
            }

            for (std::size_t i = 0; i < m_children.size(); ++i)
            {
                GameObject *child = m_children[i];
                if (child->GetEnabled())
                    child->Update();
            }
        }

        void GameObject::MarkForDestroy(void)
        {
            m_destroy = true;
        }

        void GameObject::OnInit(void)
        {
            for (Component *component : m_components)
            {
                component->VOnInit();
            }
        }

        void GameObject::OnEnabled(void)
        {
            HT_DEBUG_PRINTF("GameObject OnEnable. (not implemented)\n");
        }

        void GameObject::OnDisabled(void)
        {
            HT_DEBUG_PRINTF("GameObject OnDisable. (not implemented)\n");
        }

        GameObject* GameObject::GetChildAtIndex(std::size_t index)
        {
            if (index >= m_children.size())
                return nullptr;

            return m_children[index];
        }

        void GameObject::AddChild(GameObject *child)
        {
            if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
                m_children.push_back(child);
            child->m_transform.m_parent = &m_transform;
        }

        void GameObject::RemoveChild(GameObject* child)
        {
            auto iter = std::find(m_children.begin(), m_children.end(), child);
            if (iter != m_children.end())
                m_children.erase(iter);
            child->m_parent = nullptr;
        }

        void GameObject::RemoveChildAtIndex(std::size_t index)
        {
            if (index < m_children.size())
            {
                m_children[index]->m_transform.m_parent = nullptr;
                m_children.erase(m_children.begin() + index);
            }
        }
    }
}
