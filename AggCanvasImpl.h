#pragma once

#include <wx/dynarray.h>

#include "agg_basics.h"
#include "agg_trans_affine.h"
#include "agg_trans_viewport.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"
#include "agg_conv_curve.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_markers.h"
#include "agg_span_gradient.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_allocator.h"
#include "agg_span_converter.h"
#include "agg_span_interpolator_linear.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_gamma_functions.h"
#include "agg_scanline_u.h"
#include "agg_bezier_arc.h"
#include "agg_rounded_rect.h"
#include "agg_font_cache_manager.h"
#include "agg_font_win32_tt.h"
#include "agg_pixfmt_rgba.h"
#include "agg_image_accessors.h"
#include "agg_math.h"

typedef agg::rgba8                          AggColor;
typedef agg::order_bgra AggComponentOrder; // Platform dependent!
typedef agg::blender_rgba<AggColor, AggComponentOrder>      AggBlender;
typedef agg::rendering_buffer               AggRenderingBuffer;
typedef agg::pixfmt_alpha_blend_rgba<AggBlender, AggRenderingBuffer>    AggPixFormat;
typedef agg::renderer_base< AggPixFormat>   AggRendererBase;
typedef agg::renderer_markers<AggRendererBase>              AggRendererPrimitives;
typedef agg::renderer_scanline_aa_solid<AggRendererBase>    AggRendererSolid;
typedef agg::scanline_u8                    AggScanline;
typedef agg::rasterizer_scanline_aa<>       AggRasterizerScanline;
typedef agg::path_storage                   AggPathStorage;
typedef agg::conv_curve<AggPathStorage>     AggConvCurve;
typedef agg::conv_stroke<AggConvCurve>      AggConvStroke;
typedef agg::conv_transform<AggConvCurve>   AggPathTransform;
typedef agg::conv_transform<AggConvStroke>  AggStrokeTransform;

class wxAggCanvasImpl;
class wxAggShape
{
public:	
	enum e_shape {
		AGG_SHAPE_LINE,
		AGG_SHAPE_RECT
	};

	wxAggShape(enum e_shape shape_type): 
		m_shape_type(shape_type)
	{};
	~wxAggShape() {};

	enum e_shape m_shape_type;
	wxArrayDouble m_params;

	AggColor m_stroke_color;
	AggColor m_fill_color;
	float m_stroke_width;

	bool IsHit(int x, int y);
};

WX_DECLARE_OBJARRAY(wxAggShape, Array_AggShape);

class wxAggCanvasImpl
{
public:
	AggRenderingBuffer		m_rbuf;
    AggPixFormat			m_pixFormat;
    AggRendererBase			m_renBase;
	AggRendererPrimitives	m_renPrim;
	AggRasterizerScanline	m_rasterizer;
	AggScanline				m_scanline;
	AggRendererSolid		m_renSolid;

	AggColor	m_strokeColor;
	AggColor	m_fillColor;

	wxBitmap *mp_bitmap;
	wxArrayPtrVoid m_shapes;

	wxAggCanvasImpl();
	~wxAggCanvasImpl();

	void DrawShape(wxAggShape& shape);
};
