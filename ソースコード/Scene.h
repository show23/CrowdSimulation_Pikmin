#pragma once

#include <list>
#include<vector>
#include <typeinfo>
#include "Gameobject.h";


#define MAX_LAYER 4
class Scene
{
protected:
	std::list<GameObject*> m_GameObject[MAX_LAYER];
	

public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Init() = 0;//�������z�֐�


	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template <typename T>
	T* GetGameObject(int Layer)
	{
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)object;
			}
			
		}
		return NULL;
	}

	template <typename T>
	std::vector<T*>GetGameObjects(int Layer)
	{
		std::vector<T*> objects;
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))//�^�𒲂ׂ�(RTTI���I�^���)
			{
				objects.push_back((T*)object);
			}

		}
		return objects;
	}
	virtual void Uninit()
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (GameObject * object : m_GameObject[i])//�͈̓��[�v
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();
		}

	}
	virtual void Update()
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Update();
			}
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });//�����ɓ��Ă͂܂�Ȃ�����Bremove_if
			//�����_��
		}
		
	}

	virtual void Draw()
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
		
	}
};