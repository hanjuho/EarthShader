#pragma once



#include "hdxdefbase.h"
#include "houtputbase.h"



namespace h
{
	DLL_DECL_INHERITANCE(STRUCT, hdx_Atom, hdx_Line_FVF)
	{
		D3DXVECTOR3 dir;
		FLOAT		age;
	} *ptr_DxAtom;

	DLL_DECL_STRUCT(hdx_Particle_Info)
	{
		typedef IDirect3DVertexBuffer9* VERTEXBUFFER;
		typedef std::list<ptr_DxAtom>	ATOMS;
		hdx_3DDevice	device;
		VERTEXBUFFER	buffer;
		ATOMS			atms;
		DWORD			per_create;
		DWORD			atm_max;
		FLOAT			atm_life;
		FLOAT			atm_radius_scale;
		FLOAT			atm_radius_min;
		FLOAT			atm_radius_max;
		FLOAT			create_cycle;
	} *ptr_Particle_Info;

	DLL_DECL_INTERFACE(hdx_Particle_Controller)
	{
		INTERFACE_DECL_FUNC(create_Atom)			(ptr_DxAtom * atom)PURE;
		INTERFACE_DECL_FUNC(destroy_Atom)			(ptr_DxAtom atom)PURE;
		INTERFACE_DECL_FUNC_T(VOID, update_Atom)	(DWORD index, ptr_DxAtom atom)PURE;
	} *ptr_DxParticle_Controller;

	DLL_BASE_INHERITANCE(hdx_Particle, h_Renderer)
	{
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST DWORD atm_max = 256)PURE;
		INTERFACE_DECL_FUNC_T(VOID, update)			(CONST FLOAT delta_time, ptr_DxParticle_Controller controller)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_PerCreate)	(CONST DWORD num)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_AtomMax)	(CONST DWORD atm_max)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_AtomLife)	(CONST FLOAT time)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_AtomRadius)	(CONST FLOAT scale, CONST FLOAT min, CONST FLOAT max)PURE;
		INTERFACE_DECL_FUNC_T(VOID, set_CreateCycle)(CONST FLOAT time)PURE;
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_Particle_Info info)CONST PURE;
	} *ptr_DxParticle;

	DLL_DECL_INHERITANCE(CLASS, hdx_Particle_Own, hdx_Particle)
	{
	public:
		CLASS_DECL_STRUCTOR(hdx_Particle_Own)		(VOID);
		INTERFACE_DECL_FUNC(initialize)				(CONST hdx_3DDevice device, CONST DWORD atm_max = 256);
		INTERFACE_DECL_FUNC_T(VOID, update)			(CONST FLOAT delta_time, ptr_DxParticle_Controller controller = NULL);
		INTERFACE_DECL_FUNC_T(VOID, set_PerCreate)	(CONST DWORD num);
		INTERFACE_DECL_FUNC_T(VOID, set_AtomMax)	(CONST DWORD atm_max);
		INTERFACE_DECL_FUNC_T(VOID, set_AtomLife)	(CONST FLOAT time);
		INTERFACE_DECL_FUNC_T(VOID, set_AtomRadius)	(CONST FLOAT scale, CONST FLOAT min, CONST FLOAT max);
		INTERFACE_DECL_FUNC_T(VOID, set_CreateCycle)(CONST FLOAT time);
		INTERFACE_DECL_FUNC_T(VOID, get_info)		(ptr_Particle_Info info)CONST;
		INTERFACE_DECL_FUNC(render_before)			(VOID);
		INTERFACE_DECL_FUNC(render_after)			(VOID);
		INTERFACE_DECL_FUNC(render)					(VOID);
		INTERFACE_DECL_FUNC(reset)					(VOID);
	private:
		hdx_3DDevice					my_device;
		hdx_Particle_Info::VERTEXBUFFER	my_buffer;
		hdx_Particle_Info::ATOMS		my_atms;
		DWORD							my_per_create;
		DWORD							my_atm_max;
		FLOAT							my_atm_life;
		FLOAT							my_atm_radius_scale;
		FLOAT							my_atm_radius_min;
		FLOAT							my_atm_radius_max;
		FLOAT							my_create_time;
		FLOAT							my_create_cycle;
	} *ptr_DxParticle_Own;
};