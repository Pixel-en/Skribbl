#pragma once
#include <DirectXMath.h>
#include <list>
#include <string>
#include <assert.h>
#include "Transform.h"
#include <DxLib.h>
#include "time.h"

using namespace DirectX;

//-----------------------------------------------------------
//全てのゲームオブジェクト（シーンも含めて）が継承するインターフェース
// ゲームオブジェクトは、親子構造になっていて、
// マトリクスの影響を受けることになる
//-----------------------------------------------------------
class GameObject
{

protected:
	//位置や向きなどを管理するオブジェクト
	Transform				transform_;

	//オブジェクトの名前
	std::string				objectName_;

public:
	//コンストラクタ
	GameObject();
	GameObject(GameObject* parent);
	GameObject(GameObject* parent, const std::string& name);

	//デストラクタ
	virtual ~GameObject();

	//各オブジェクトで必ず作る関数
	virtual void Initialize(void) {}
	virtual void Update(void) {}
	virtual void Draw() {}
	virtual void Release(void) {}

	//自分の該当関数を読んだ後、子供の関数も呼ぶ
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();


	////ローカル行列の取得（このオブジェクトの行列）
	////戻値：ローカル行列
	//XMMATRIX GetLocalMatrix();

	//ワールド行列の取得（親の影響を受けた最終的な行列）
	//戻値：ワールド行列
	XMMATRIX GetWorldMatrix();



	//各フラグの制御
	bool IsDead();			// 削除するかどうか
	void KillMe();			// 自分を削除する
	void Enter();			// Updateを許可
	void Leave();			// Updateを拒否
	void Visible();			// Drawを許可
	void Invisible();		// Drawを拒否
	bool IsInitialized();	// 初期化済みかどうか
	void SetInitialized();	// 初期化済みにする
	bool IsEntered();		// Update実行していいか
	bool IsVisibled();		// Draw実行していいか


	//子オブジェクトリストを取得
	//戻値：子オブジェクトリスト
	std::list<GameObject*>* GetChildList();

	//親オブジェクトを取得
	//戻値：親オブジェクトのアドレス
	GameObject* GetParent();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：name	検索する名前
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	GameObject* FindChildObject(const std::string& name);

	//クラス名でオブジェクトを検索（対象は自分の子供以下）
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	template<class C>
	C* FindGameObject() {
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				return ret;
			}
			else {
				C* c = obj->FindGameObject<C>();
				if (c != nullptr)
					return c;
			}
		}
		return nullptr;
	}

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：tag	検索するタグ
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	template<class C>
	C* FindGameObject(const std::string& name) {
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				if (obj->GetObjectName() == name) {
					return ret;
				}
			}
			else {
				C* c = obj->FindGameObject<C>(name);
				if (c != nullptr)
					return c;
			}
		}
		return nullptr;
	}

	//クラス名でオブジェクトを検索（対象は自分の子供以下）
	//戻値：見つけたオブジェクトのリスト
	template<class C>
	std::list<C*> FindGameObjects() {
		std::list<C*> rets;
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				rets.push_back(ret);
			}
			else {
				std::list<C*> childList = obj->FindGameObjects<C>();
				for (C* I : childList)
					rets.push_back(I);
			}
		}
		return rets;
	}

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：tag	検索するタグ
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	template<class C>
	std::list<C*> FindGameObjects(const std::string& name) {
		std::list<C*> rets;
		auto list = GetChildList();
		for (GameObject* obj : *list) {
			C* ret = dynamic_cast<C*>(obj);
			if (ret != nullptr) {
				if (obj->GetObjectName() == name) {
					rets.push_back(ret);
				}
			}
			else {
				std::list<C*> childList = obj->FindGameObjects<C>();
				for (C* I : childList)
					rets.push_back(I);
			}
		}
		return rets;
	}

	//名前でオブジェクトを検索（対象は全体）
	//引数：検索する名前
	//戻値：見つけたオブジェクトのアドレス
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//オブジェクトの名前を取得
	//戻値：名前
	const std::string& GetObjectName(void) const;

	//子オブジェクトを追加（リストの最後へ）
	//引数：追加するオブジェクト
	void PushBackChild(GameObject* obj);

	//子オブジェクトを追加（リストの先頭へ）
	//引数：obj 追加するオブジェクト
	void PushFrontChild(GameObject* obj);

	//子オブジェクトを全て削除
	void KillAllChildren();
	//RootJobを取得
	GameObject* GetRootJob();

	//各アクセス関数
	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }
	XMFLOAT3 GetWorldRotate() { return Transform::Float3Add(GetParent()->transform_.rotate_, transform_.rotate_); }
	XMFLOAT3 GetWorldScale() { return Transform::Float3Add(GetParent()->transform_.scale_, transform_.scale_); }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3(x, y, z)); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3(x, y, z)); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }


private:

	//オブジェクト削除（再帰）
	//引数：obj　削除するオブジェクト
	void KillObjectSub(GameObject* obj);

private:
	//フラグ
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;			//削除するか
	};
	OBJECT_STATE state_;

	//親オブジェクト
	GameObject* pParent_;

	//子オブジェクトリスト
	std::list<GameObject*> childList_;

	//便利関数系を作ったら入れておきたい
public:

	/// <summary>
	/// 値を丸めて返す
	/// </summary>
	/// <typeparam name="T">型(boolは動作未定義)</typeparam>
	/// <param name="_val">丸める値</param>
	/// <param name="_min">最小値</param>
	/// <param name="_max">最大値</param>
	/// <returns>丸めた値</returns>
	template <typename T>
	T Clamp(T _val, T _min, T _max) {
		if (_min > _max)
			return 0;

		if (_val <= _min)
			return _min;
		else if (_val >= _max)
			return _max;
		return _val;
	}

protected:

	//ハンドルがあるかチェック
	//引数：handle　調べるハンドル
	//引数：str ハンドルがない時のエラーメッセージ
	void HandleCheck(int handle, std::string str);

};


//オブジェクトを作成するテンプレート
template <class T>
T* Instantiate(GameObject* pParent)
{
	T* pNewObject = new T(pParent);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}

