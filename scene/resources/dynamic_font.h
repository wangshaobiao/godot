#ifndef DYNAMIC_FONT_H
#define DYNAMIC_FONT_H

#ifdef FREETYPE_ENABLED
#include "scene/resources/font.h"
#include "os/thread_safe.h"
#include "io/resource_loader.h"

#include <ft2build.h>
#include FT_FREETYPE_H


class DynamicFontAtSize;
class DynamicFont;

class DynamicFontData : public Resource {

	OBJ_TYPE(DynamicFontData,Resource);



	const uint8_t *font_mem;
	int font_mem_size;
	bool force_autohinter;

	String font_path;
	Map<int,DynamicFontAtSize*> size_cache;

	friend class DynamicFontAtSize;

friend class DynamicFont;


	Ref<DynamicFontAtSize> _get_dynamic_font_at_size(int p_size);
public:

	void set_font_ptr(const uint8_t* p_font_mem,int p_font_mem_size);
	void set_font_path(const String& p_path);
	void set_force_autohinter(bool p_force);

	DynamicFontData();
	~DynamicFontData();
};


class DynamicFontAtSize : public Reference {

	OBJ_TYPE(DynamicFontAtSize,Reference)

	_THREAD_SAFE_CLASS_

	FT_Library   library;   /* handle to library     */
	FT_Face      face;      /* handle to face object */
	FT_StreamRec stream;

	int ascent;
	int descent;
	int linegap;
	int rect_margin;

	bool valid;

	struct CharTexture {

		DVector<uint8_t> imgdata;
		int texture_size;
		Vector<int> offsets;
		Ref<ImageTexture> texture;
	};

	Vector<CharTexture> textures;

	struct Character {

		int texture_idx;
		Rect2 rect;
		float v_align;
		float h_align;
		float advance;

		Character() { texture_idx=0; v_align=0; }
	};


	static unsigned long _ft_stream_io(FT_Stream      stream,  unsigned long   offset,  unsigned char*  buffer,  unsigned long   count );
	static void _ft_stream_close(FT_Stream       stream);

	HashMap< CharType, Character > char_map;

	_FORCE_INLINE_ void _update_char(CharType p_char);

friend class DynamicFontData;
	Ref<DynamicFontData> font;
	int size;



	Error _load();
protected:



public:


	float get_height() const;

	float get_ascent() const;
	float get_descent() const;

	Size2 get_char_size(CharType p_char,CharType p_next=0) const;

	float draw_char(RID p_canvas_item, const Point2& p_pos, const CharType& p_char,const CharType& p_next=0,const Color& p_modulate=Color(1,1,1)) const;



	DynamicFontAtSize();
	~DynamicFontAtSize();
};

///////////////

class DynamicFont : public Font {

	OBJ_TYPE( DynamicFont, Font );

	Ref<DynamicFontData> data;
	Ref<DynamicFontAtSize> data_at_size;
	int size;
	bool valid;

protected:

	static void _bind_methods();

public:

	void set_font_data(const Ref<DynamicFontData>& p_data);
	Ref<DynamicFontData> get_font_data() const;

	void set_size(int p_size);
	int get_size() const;

	virtual float get_height() const;

	virtual float get_ascent() const;
	virtual float get_descent() const;

	virtual Size2 get_char_size(CharType p_char,CharType p_next=0) const;

	virtual bool is_distance_field_hint() const;

	virtual float draw_char(RID p_canvas_item, const Point2& p_pos, const CharType& p_char,const CharType& p_next=0,const Color& p_modulate=Color(1,1,1)) const;

	DynamicFont();
	~DynamicFont();

};



/////////////

class ResourceFormatLoaderDynamicFont : public ResourceFormatLoader {
public:

	virtual RES load(const String &p_path,const String& p_original_path="",Error *r_error=NULL);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String& p_type) const;
	virtual String get_resource_type(const String &p_path) const;

};


#endif

#endif
