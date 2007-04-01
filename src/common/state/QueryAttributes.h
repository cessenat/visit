#ifndef QUERYATTRIBUTES_H
#define QUERYATTRIBUTES_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>
#include <visitstream.h>

// ****************************************************************************
// Class: QueryAttributes
//
// Purpose:
//    This class contains attributes used for query.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   Fri Feb 20 15:56:07 PST 2004
//
// Modifications:
//   
// ****************************************************************************

class STATE_API QueryAttributes : public AttributeSubject
{
public:
    enum ElementType
    {
        Zone,
        Node
    };
    enum VarType
    {
        Mesh,
        Scalar,
        Vector,
        Tensor,
        Symmetric_Tensor,
        Material,
        Species,
        Curve,
        Unknown
    };
    enum DataType
    {
        ActualData,
        OriginalData
    };

    QueryAttributes();
    QueryAttributes(const QueryAttributes &obj);
    virtual ~QueryAttributes();

    virtual void operator = (const QueryAttributes &obj);
    virtual bool operator == (const QueryAttributes &obj) const;
    virtual bool operator != (const QueryAttributes &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectName();
    void SelectVariables();
    void SelectResultsMessage();
    void SelectWorldPoint();
    void SelectResultsValue();
    void SelectVarTypes();

    // Property setting methods
    void SetName(const std::string &name_);
    void SetVariables(const stringVector &variables_);
    void SetResultsMessage(const std::string &resultsMessage_);
    void SetWorldPoint(const float *worldPoint_);
    void SetDomain(int domain_);
    void SetElement(int element_);
    void SetResultsValue(const doubleVector &resultsValue_);
    void SetElementType(ElementType elementType_);
    void SetTimeStep(int timeStep_);
    void SetVarTypes(const intVector &varTypes_);
    void SetDataType(DataType dataType_);
    void SetPipeIndex(int pipeIndex_);

    // Property getting methods
    const std::string  &GetName() const;
          std::string  &GetName();
    const stringVector &GetVariables() const;
          stringVector &GetVariables();
    const std::string  &GetResultsMessage() const;
          std::string  &GetResultsMessage();
    const float        *GetWorldPoint() const;
          float        *GetWorldPoint();
    int                GetDomain() const;
    int                GetElement() const;
    const doubleVector &GetResultsValue() const;
          doubleVector &GetResultsValue();
    ElementType        GetElementType() const;
    int                GetTimeStep() const;
    const intVector    &GetVarTypes() const;
          intVector    &GetVarTypes();
    DataType           GetDataType() const;
    int                GetPipeIndex() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string ElementType_ToString(ElementType);
    static bool ElementType_FromString(const std::string &, ElementType &);
protected:
    static std::string ElementType_ToString(int);
public:
    static std::string VarType_ToString(VarType);
    static bool VarType_FromString(const std::string &, VarType &);
protected:
    static std::string VarType_ToString(int);
public:
    static std::string DataType_ToString(DataType);
    static bool DataType_FromString(const std::string &, DataType &);
protected:
    static std::string DataType_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    void Reset();
    void PrintSelf(ostream &os);
    void SetResultsValue(const double);
    void SetResultsValues(const double*, const int);
private:
    std::string  name;
    stringVector variables;
    std::string  resultsMessage;
    float        worldPoint[3];
    int          domain;
    int          element;
    doubleVector resultsValue;
    int          elementType;
    int          timeStep;
    intVector    varTypes;
    int          dataType;
    int          pipeIndex;
};

#endif
