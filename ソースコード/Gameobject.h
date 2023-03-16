#pragma once

class GameObject
{
protected:
	bool m_Destroy = false;
	
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Velocity;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Random;
	float MinLength;

	
	int m_Number;
	

public:
	GameObject() {}//コンストラクタ
	virtual ~GameObject(){}//デストラクタ
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPosition(D3DXVECTOR3 Position)
	{
		m_Position = Position;
	}
	void SetRotation(D3DXVECTOR3 Rotation)
	{
		m_Rotation = Rotation;
	}
	void SetVelocity(D3DXVECTOR3 Velocity)
	{
		m_Velocity = Velocity;
	}
	
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	void SetDestroy() { m_Destroy = true; }
	
	int GetNumber() { return m_Number; }
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	
	//マトリクス
	//11 12 13 14    11 12 13はx（右）
	//21 22 23 24    21 22 23はy (上）
	//31 32 33 34    31 32 33はz (前)
	//41 42 43 44
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

};