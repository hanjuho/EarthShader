#pragma once



#pragma comment(lib, "hdevstdlib.lib")



#include <hdxdefbase.h>
#include <hdxlineweb.h>
#include <hdxparticle.h>
#include <hdxmesh.h>
#include <hdxinput.h>
#include <hdxcamera.h>
#include <hembedded.h>



#define CO_METER	12756.2
#define METER_UNIT	1/CO_METER
#define G_EARTH		2.491



namespace h
{
	static D3DXMATRIX M, T, R, S;

	/*
	행성이 움직이는데 필요한 최소 정보를 가지고 있다.
	*/
	DECL_STRUCT(Cosmos)
	{
		CLASS_DECL_CONSTRUCTOR(Cosmos)(
			LPCWSTR name = TEXT(""), DWORD type = 0,
			DOUBLE radius = 0.0, DOUBLE mass = 0.0,
			DOUBLE avx = 0.0, DOUBLE avy = 0.0, DOUBLE avz = 0.0,
			DOUBLE vx = 0.0, DOUBLE vy = 0.0, DOUBLE vz = 0.0,
			DOUBLE x = 0.0, DOUBLE y = 0.0, DOUBLE z = 0.0,
			DOUBLE t = 0.0, DOUBLE rx = 0.0, DOUBLE ry = 0.0, DOUBLE rz = 0.0);
		std::wstring	name;
		DWORD			type;
		DOUBLE			radius;
		DOUBLE			mass;
		DOUBLE			acc_velocity[3];
		DOUBLE			velocity[3];
		DOUBLE			position[3];
		DOUBLE			tangle;
		DOUBLE			torque;
		DOUBLE			tAxis[3];
	} *ptr_Cosmos;

	/*
	행성은 자신의 모성이 x축 상에 나란히 올려져 있다는 가정하에 계산된다.
	행성의 공전방향은 x축으로 궤도 각도만큼 회전된 z축이다.
	행성의 자전방향은 z축으로 자전 각도만큼 회전된 y축이다.
	*/
	DECL_FUNC_T(ptr_Cosmos, co_get_Sun)		(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Mercury)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Venus)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Earth)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Moon)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Mars)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Jupiter)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Saturn)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Uranus)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(ptr_Cosmos, co_get_Neptune)	(ptr_Cosmos co, CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);
	DECL_FUNC_T(DOUBLE *, co_setVector)		(DOUBLE vector[3], CONST DOUBLE x, CONST DOUBLE y, CONST DOUBLE z);

	/*
	행성을 그래픽으로 표현하기 위해 mesh를 구성한다.
	*/
	DECL_FUNC(co_init_Planet)(hdx_3DDevice device, LPCWSTR filename, ptr_DxMesh mesh);

	/*
	행성의 정보를 함수 형태로 정의한 인터페이스
	*/
	DECL_INTERFACE(Cosmos_Func)
	{
		INTERFACE_DECL_FUNC(render)							(VOID)PURE;
		INTERFACE_DECL_FUNC_T(VOID, update)					(FLOAT delta)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Name)				(LPCWSTR name)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Radius)				(DOUBLE radius)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Mass)				(DOUBLE mass)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_accVelocity)		(CONST DOUBLE buffer[3])PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Velocity)			(CONST DOUBLE buffer[3])PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Position)			(CONST DOUBLE buffer[3])PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_Torque)				(CONST DOUBLE torque)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_TAxis)				(CONST DOUBLE buffer[3])PURE;
		INTERFACE_DECL_FUNC_T(LPCWSTR, get_Name)			(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DWORD, get_Type)				(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE, get_Radius)			(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE, get_Mass)				(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE *, get_accVelocity)	(DOUBLE buffer[3])CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE *, get_Velocity)		(DOUBLE buffer[3])CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE *, get_Position)		(DOUBLE buffer[3])CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE, get_Torque)			(VOID)CONST PURE;
		INTERFACE_DECL_FUNC_T(DOUBLE *, get_TAxis)			(DOUBLE buffer[3])CONST PURE;		
	} *ptr_CosmosFunc;

	/*
	HW = Cosmos
	SW = Cosmos_Func
	행성의 움직임을 계산하고 출력하기 위한 Embedded_Pattern이다.
	*/
	typedef public h::Embedded_Architecture_Pattern<Cosmos, Cosmos_Func> Embedded_Cosmos_Parent;
	typedef class Embedded_Cosmos :
		public Embedded_Cosmos_Parent,
		public h_MouseListener,
		public h_KeyboardListener
	{
	public:

		/*
		행성을 그래픽으로 표현하기 위해 행성과 파티클을 가지고 있다.
		*/
		DECL_CLASS(Cosmos_Planet)
		{
		public:
			CLASS_DECL_STRUCTOR(Cosmos_Planet)	(VOID);
			INTERFACE_DECL_FUNC(initialize)		(hdx_3DDevice device, LPCWSTR filename);
			INTERFACE_DECL_FUNC(render_mesh)	(VOID);
			INTERFACE_DECL_FUNC(render_particle)(VOID);
			INTERFACE_DECL_FUNC_T(VOID, update)	(CONST FLOAT delta_time, ptr_DxParticle_Controller controller);
			INTERFACE_DECL_FUNC_T(VOID, set_PerCreate)	(CONST DWORD num);
			INTERFACE_DECL_FUNC_T(VOID, set_AtomMax)	(CONST DWORD atm_max);
			INTERFACE_DECL_FUNC_T(VOID, set_AtomLife)	(CONST FLOAT time);
			INTERFACE_DECL_FUNC_T(VOID, set_AtomRadius)	(CONST FLOAT min, CONST FLOAT max);
			INTERFACE_DECL_FUNC_T(VOID, set_CreateCycle)(CONST FLOAT time);
			INTERFACE_DECL_FUNC_T(hdx_3DDevice, get_Device)(VOID);
		private:
			hdx_3DDevice		my_device;
			hdx_Mesh_Own		my_mesh;
			hdx_Particle_Own	my_particle;
		public:
			D3DXMATRIX			my_form;
		} *ptr_Cosmos_Planet;

		/*
		Embedded_Cosmos 만을 위한 Cosmos_Func
		*/
		DECL_EXINHERITANCE(CLASS, Cosmos_SW, SW, hdx_Particle_Controller)
		{
		public:
			CLASS_DECL_CONSTRUCTOR(Cosmos_SW)	(DWORD id);
			// My
			INTERFACE_DECL_FUNC(initialize)		(ptr_Cosmos hw, ptr_Cosmos_Planet planet);
			// SW
			INTERFACE_DECL_FUNC(invalid)		(VOID);
			// Cosmos_Func
			INTERFACE_DECL_FUNC(render)			(VOID);
			INTERFACE_DECL_FUNC_T(VOID, update)				(FLOAT delta);
			INTERFACE_DECL_FUNC_T(VOID, set_Name)			(LPCWSTR name);
			INTERFACE_DECL_FUNC_T(VOID, set_Radius)			(DOUBLE radius);
			INTERFACE_DECL_FUNC_T(VOID, set_Mass)			(DOUBLE mass);
			INTERFACE_DECL_FUNC_T(VOID, set_accVelocity)	(CONST DOUBLE buffer[3]);
			INTERFACE_DECL_FUNC_T(VOID, set_Velocity)		(CONST DOUBLE buffer[3]);
			INTERFACE_DECL_FUNC_T(VOID, set_Position)		(CONST DOUBLE buffer[3]);
			INTERFACE_DECL_FUNC_T(VOID, set_Torque)			(CONST DOUBLE torque);
			INTERFACE_DECL_FUNC_T(VOID, set_TAxis)			(CONST DOUBLE buffer[3]);
			INTERFACE_DECL_FUNC_T(LPCWSTR, get_Name)		(VOID)CONST;
			INTERFACE_DECL_FUNC_T(DWORD, get_Type)			(VOID)CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE, get_Radius)		(VOID)CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE, get_Mass)			(VOID)CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE *, get_accVelocity)	(DOUBLE buffer[3])CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE *, get_Velocity)		(DOUBLE buffer[3])CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE *, get_Position)		(DOUBLE buffer[3])CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE, get_Torque)			(VOID)CONST;
			INTERFACE_DECL_FUNC_T(DOUBLE *, get_TAxis)			(DOUBLE buffer[3])CONST;
			// hdx_Particle_Controller
			INTERFACE_DECL_FUNC(create_Atom)			(ptr_DxAtom * atom);
			INTERFACE_DECL_FUNC(destroy_Atom)			(ptr_DxAtom atom);
			INTERFACE_DECL_FUNC_T(VOID, update_Atom)	(DWORD index, ptr_DxAtom atom);
		protected:
			ptr_Cosmos			my_cosmos;
			ptr_Cosmos_Planet	my_planet;
		} *ptr_Cosmos_SW;

		/*
		태양 만을 위한 확장 Cosmos_Func
		*/
		DECL_INHERITANCE(CLASS, Star_SW, Cosmos_SW)
		{
		public:
			CLASS_DECL_CONSTRUCTOR(Star_SW)				(DWORD id);
			INTERFACE_DECL_FUNC(initialize)				(ptr_Cosmos hw, ptr_Cosmos_Planet planet);
			INTERFACE_DECL_FUNC(render)					(VOID);
			INTERFACE_DECL_FUNC(create_Atom)			(ptr_DxAtom * atom);
			INTERFACE_DECL_FUNC_T(VOID, update_Atom)	(DWORD index, ptr_DxAtom atom);
		protected:
			hdx_Light my_light;
		} *ptr_Star_SW;

		/*
		지구 만을 위한 확장 Cosmos_Func
		*/
		DECL_INHERITANCE(CLASS, Earth_SW, Cosmos_SW)
		{
		public:
			CLASS_DECL_STRUCTOR(Earth_SW)		(DWORD id);
			INTERFACE_DECL_FUNC(initialize)		(ptr_Cosmos hw, ptr_Cosmos_Planet planet);
			INTERFACE_DECL_FUNC(render)			(VOID);
			INTERFACE_DECL_FUNC_T(VOID, update)	(FLOAT delta);
		protected:
			hdx_PixelShader		my_pixel;
			LPDIRECT3DTEXTURE9	my_specTex;
			LPDIRECT3DTEXTURE9	my_bumpTex;
			D3DXCONSTANT_DESC	my_baseTexDesc;
			D3DXCONSTANT_DESC	my_specTexDesc;
			D3DXCONSTANT_DESC	my_bumpTexDesc;
			D3DXHANDLE			my_baseTexHandle;
			D3DXHANDLE			my_specTexHandle;
			D3DXHANDLE			my_bumpTexHandle;
			D3DXHANDLE			my_attractionHandle;
			D3DXHANDLE			my_gravityHandle;
			D3DXVECTOR4			my_attraction;
		} *ptr_Earth_SW;

		/*
		Embedded_Cosmos 만을 위한 Factory
		*/
		DECL_INHERITANCE(CLASS, Cosmos_Factory,	Factory)
		{
			CLASS_DECL_OPEN_FUNC_T(DWORD, open_Create_ID)(VOID);
		public:
			typedef	std::vector<Cosmos_Planet> PLANETS;
			CLASS_DECL_STRUCTOR(Cosmos_Factory)		(VOID);
			INTERFACE_DECL_FUNC(initialize)			(hdx_3DDevice device);
			INTERFACE_DECL_FUNC(construct_hw)		(CONST KEY key, CONST DWORD option, DWORD * id, ptr_Cosmos hw)CONST;
			INTERFACE_DECL_FUNC(destruct_hw)		(CONST KEY  key, CONST DWORD id, ptr_Cosmos hw)CONST;
			INTERFACE_DECL_FUNC(develop_sw)			(CONST ptr_Cosmos hw, CONST DWORD id, ptr_SW * pSw)CONST;
			INTERFACE_DECL_FUNC_T(DWORD, create_id)	(CONST KEY key, CONST DWORD option)CONST;
			INTERFACE_DECL_FUNC_T(DWORD, get_type)	(VOID)CONST;
		private:
			PLANETS my_planets;
		} *ptr_Cosmos_Factory;

		/*
		Embedded_Cosmos 만을 위한 web
		*/
		DECL_EXINHERITANCE(CLASS, Cosmos_Web, Pattern_Buffer, hdx_LineWeb_Controller)
		{
		public:
			CLASS_DECL_STRUCTOR(Cosmos_Web)	(VOID);
			INTERFACE_DECL_FUNC(initialize)	(hdx_3DDevice device);
			INTERFACE_DECL_FUNC(render)		(VOID);
			INTERFACE_DECL_FUNC(focus)		(DWORD focus);
			INTERFACE_DECL_FUNC_T(VOID, update_FVF)		(DWORD index, ptr_DxLine_FVF fvf);
			INTERFACE_DECL_FUNC_T(VOID, update_Line)	(DWORD index, ptr_DxLine_Pair pair);
		private:
			DWORD			my_focus;
			hdx_LineWeb_Own	my_web;
		} *ptr_Cosmos_Buffer;

		CLASS_DECL_STRUCTOR(Embedded_Cosmos)(VOID);
		INTERFACE_DECL_FUNC(initialize)						(hdx_3DDevice device);
		INTERFACE_DECL_FUNC(render)							(VOID);
		INTERFACE_DECL_FUNC_T(VOID, update)					(FLOAT delta);
		INTERFACE_DECL_FUNC_T(ptr_F3DCamera, get_Camera)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(ptr_Factory, get_Factory)		(VOID)CONST;
		INTERFACE_DECL_FUNC_T(ptr_Container, get_Container)	(VOID)CONST;
		INTERFACE_DECL_FUNC_T(ptr_Finder, get_Finder)		(VOID)CONST;
		CLASS_DECL_FUNC_T(ptr_SW, get_FocusSW)				(VOID)CONST;
		CLASS_DECL_FUNC_T(iterator, create_iterator)		(VOID)CONST;

		INTERFACE_DECL_FUNC_T(VOID, on_KeyState)(ptr_KeyboardEvent key);
		INTERFACE_DECL_FUNC_T(VOID, on_ClickDown)(ptr_MouseEvent e);
		INTERFACE_DECL_FUNC_T(VOID, on_ClickUp)(ptr_MouseEvent e);
		INTERFACE_DECL_FUNC_T(VOID, on_Drag)(ptr_MouseEvent e);
		INTERFACE_DECL_FUNC_T(VOID, on_Move)(ptr_MouseEvent e);
		INTERFACE_DECL_FUNC_T(VOID, on_Wheel)(ptr_MouseEvent e);

	protected:
		CLASS_DECL_FUNC_T(VOID, camera_initialize)	(ptr_SW focus);
		CLASS_DECL_FUNC_T(VOID, camera_Look)		(VOID);
		CLASS_DECL_FUNC_T(VOID, camera_Rotate)		(CONST FLOAT x, CONST FLOAT y);
		CLASS_DECL_FUNC_T(VOID, camera_Zoom)		(CONST FLOAT z);

	private:
		Cosmos_Factory		my_factory;
		Pattern_Container	my_container;
		Pattern_Finder		my_finder;
		Cosmos_Web			my_buffer;
		hdx_3DDevice		my_device;

		hdx_F3DCamera		my_camera;
		ptr_SW				my_focusSW;
		D3DXVECTOR3			my_dir;
		FLOAT				my_length;

		FLOAT				my_timeAccel;
	} *ptr_Embedded_Cosmos;
};