Implications of using 0 to denote an implicit dimension.
    * std::extent<> returns 0 when out of range or operating on non-array types.
    * More importantly, an extent of 0 is relevant when dealing with padding;
      dimensions<0, 0, 0> would be the ideal way to specify no padding for a
      3D array_ref<>, but we'd like to avoid having it by dynamic.
    * Suggestion: Add std::experimental::dyn, which is a non-zero magic value.
Interface between properties and array_ref<> is not currently well defined,
which will limit user extensibility.
    * The interface between properties and array_ref<> needs to be well defined;
      currently we have opaque tag types.
Semantics and interface for properties interface needs further definition.
    * Behavior for multiple layouts?
    * Ordering of properties?
    * Interactions of properties that affect the same "policy"?
        * atomic + lives on gpu example.
    * Suggestion: Explicit specify clearly defined "policies" that we care
      about, based on what's in Kokkos. Each property fulfills one policy,
      and no more than one property per policy should be specified for one
      array_ref<>.
        * E.g. the four types of properties Carter + Bryce discussed on Monday
          would be policies.
        * How important is it that we're able to specify properties in any order?
        * Users can use variadic template parameters in functions dealing with
          generic array_refs, so no changes are needed for that use case. 
Semantics and interface for layout properties needs further definition.
    * We need layouts which:
        A.) Do not do any padding or striding.
        B.) Do padding and striding which is determined by the implementation (maybe).
        C.) Pad to a certain alignment (with explicit striding?).
        A.) Allow padding and striding to be explicitly specified.
    * Bryce is still unclear on use case for layout_stride, will need to go over it with Carter again.

Language for type of dimensions needs to be unified - currently the spec uses
explicit/implicit dimensions (mostly), and the implementation uses
static/dynamic.

