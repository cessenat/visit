#ifndef ANNOTATIONOBJECT_H
#define ANNOTATIONOBJECT_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>
#include <ColorAttribute.h>

// ****************************************************************************
// Class: AnnotationObject
//
// Purpose:
//    This class defines a general set of attributes that are used to set the attributes for all annotation objects.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   Mon Nov 10 09:20:12 PDT 2003
//
// Modifications:
//   
// ****************************************************************************

class STATE_API AnnotationObject : public AttributeSubject
{
public:
    enum AnnotationType
    {
        Text2D,
        Text3D,
        TimeSlider,
        Arrow2D,
        Arrow3D,
        Box,
        Image
    };
    enum FontFamily
    {
        Arial,
        Courier,
        Times
    };

    AnnotationObject();
    AnnotationObject(const AnnotationObject &obj);
    virtual ~AnnotationObject();

    virtual void operator = (const AnnotationObject &obj);
    virtual bool operator == (const AnnotationObject &obj) const;
    virtual bool operator != (const AnnotationObject &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectPosition();
    void SelectPosition2();
    void SelectTextColor();
    void SelectColor1();
    void SelectColor2();
    void SelectText();

    // Property setting methods
    void SetObjectType(AnnotationType objectType_);
    void SetVisible(bool visible_);
    void SetActive(bool active_);
    void SetPosition(const float *position_);
    void SetPosition2(const float *position2_);
    void SetTextColor(const ColorAttribute &textColor_);
    void SetUseForegroundForTextColor(bool useForegroundForTextColor_);
    void SetColor1(const ColorAttribute &color1_);
    void SetColor2(const ColorAttribute &color2_);
    void SetText(const stringVector &text_);
    void SetFontFamily(FontFamily fontFamily_);
    void SetFontBold(bool fontBold_);
    void SetFontItalic(bool fontItalic_);
    void SetFontShadow(bool fontShadow_);
    void SetFloatAttribute1(float floatAttribute1_);
    void SetIntAttribute1(int intAttribute1_);

    // Property getting methods
    AnnotationType       GetObjectType() const;
    bool                 GetVisible() const;
    bool                 GetActive() const;
    const float          *GetPosition() const;
          float          *GetPosition();
    const float          *GetPosition2() const;
          float          *GetPosition2();
    const ColorAttribute &GetTextColor() const;
          ColorAttribute &GetTextColor();
    bool                 GetUseForegroundForTextColor() const;
    const ColorAttribute &GetColor1() const;
          ColorAttribute &GetColor1();
    const ColorAttribute &GetColor2() const;
          ColorAttribute &GetColor2();
    const stringVector   &GetText() const;
          stringVector   &GetText();
    FontFamily           GetFontFamily() const;
    bool                 GetFontBold() const;
    bool                 GetFontItalic() const;
    bool                 GetFontShadow() const;
    float                GetFloatAttribute1() const;
    int                  GetIntAttribute1() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string AnnotationType_ToString(AnnotationType);
    static bool AnnotationType_FromString(const std::string &, AnnotationType &);
protected:
    static std::string AnnotationType_ToString(int);
public:
    static std::string FontFamily_ToString(FontFamily);
    static bool FontFamily_FromString(const std::string &, FontFamily &);
protected:
    static std::string FontFamily_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

private:
    int            objectType;
    bool           visible;
    bool           active;
    float          position[3];
    float          position2[3];
    ColorAttribute textColor;
    bool           useForegroundForTextColor;
    ColorAttribute color1;
    ColorAttribute color2;
    stringVector   text;
    int            fontFamily;
    bool           fontBold;
    bool           fontItalic;
    bool           fontShadow;
    float          floatAttribute1;
    int            intAttribute1;
};

#endif
