#include<type_traits>
#include<cstdio>

template<class ValueType>
struct restrict_reference {
  ValueType& ref;
  restrict_reference(ValueType& ref_):ref(ref_) {} 

  void operator+= (const ValueType& upd) {
    printf("RestrictReference %p\n",&ref);
  }
};

template<class ValueType>
struct atomic_reference {
  ValueType& ref;
  atomic_reference(ValueType& ref_):ref(ref_) {}

  void operator+= (const ValueType& upd) {
    printf("AtomicReference %p\n",&ref);
  }
};

// We need some struct to start the AccessPropertie extraction
// We can also use this to limit implementation size of real properties by always adding it as the last property again. 
struct null_access_property {
  
  template<class ValueType, class ... AccessProperties>
  struct reference_type_adapter;

  template<class ValueType, class Property1, class ... AccessProperties>
  struct reference_type_adapter<ValueType,Property1,AccessProperties...> {
    typedef typename Property1:: template reference_type_adapter<ValueType, AccessProperties...>::type prop1_ref_t_adapt;
    static constexpr bool required =  prop1_ref_t_adapt::required;
    typedef typename prop1_ref_t_adapt::reference_type reference_type;   
    typedef reference_type_adapter<ValueType,Property1,AccessProperties...> type;
  };

  // Stop the recursion chain  
  template<class ValueType>
  struct reference_type_adapter<ValueType> {
    static constexpr bool required = false ;
    typedef ValueType& reference_type;
    typedef reference_type_adapter<ValueType> type;
  };

};

struct restrict_access {
  
  template<class ValueType, class Property1, class ... AccessProperties>
  struct reference_type_adapter {
    private:
    // Typedef Nested Property Adapter
    typedef typename Property1:: template reference_type_adapter<ValueType,AccessProperties...>::type 
      prop1_ref_t_adapt;

    public:
    // This chain the required property
    static constexpr bool required =  prop1_ref_t_adapt::required;

    // If no other property is required use restrict_reference (i.e. drop other properties)
    typedef typename std::conditional<required,typename prop1_ref_t_adapt::reference_type,restrict_reference<ValueType> >::type reference_type;  

    // Define the selftype
    typedef reference_type_adapter<ValueType,Property1,AccessProperties...> type;

  };

};

struct atomic_access {
  
  template<class ValueType, class Property1, class ... AccessProperties>
  struct reference_type_adapter {
    private:
    // Typedef Nested Property Adapter
    typedef typename Property1:: template reference_type_adapter<ValueType,AccessProperties...>::type 
      prop1_ref_t_adapt;

    // Make sure no subsequent properties are required
    static_assert(!prop1_ref_t_adapt::required,"Multiple Required AccessProperties");

    public:
    // This property can not be ignored
    static constexpr bool required =  true;
    
    // This adapter just uses the atomic reference
    typedef atomic_reference<ValueType> reference_type;
    
    // Define the selftype
    typedef reference_type_adapter<ValueType,Property1,AccessProperties...> type;
  };

};

template<class ValueType, class ... AccessProperties>
class span {
public:
  typedef typename null_access_property::reference_type_adapter<ValueType,AccessProperties...,null_access_property>::reference_type reference_type;
  typedef ValueType* ptr_type; 

  span(ptr_type ptr_,ptrdiff_t size_):ptr(ptr_),size(size_) {
  }
  reference_type operator() (const int& i) {
    return reference_type(ptr[i]);
  }

private:
  ptr_type ptr;
  ptrdiff_t size;
};


// AccessProperties:
// Only one of the AccessProperties is allowed to be required
// NonRequired AccessProperties are allowed to be ignored
// The reference_type of span may be order dependent for the AccessProperties

int main() {

  double* vals = new double[100];
  span<double> a(vals,100);
  span<double,restrict_access> b(vals,100);
  span<double,atomic_access> c(vals,100);
  span<double,atomic_access,restrict_access> d(vals,100);
  span<double,restrict_access,atomic_access> e(vals,100);
 
  // Triggers Static Assert:
  //span<double,atomic_access,restrict_access,atomic_access> f(vals,100);
 

  printf("Ptr: %p\n",vals+7);
  a(7)+=10.0;
  b(7)+=10.0;
  c(7)+=10.0;
  d(7)+=10.0;
  e(7)+=10.0;

}

