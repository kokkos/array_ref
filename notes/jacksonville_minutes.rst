Discussion of magic value for implicit dimensions
    * 0 vs some other value (e.g. -1)
    * Axil: Is there a valid use case for a dimension<> with 0 extents?
        * Bryce: Yes, expressing no padding - you might want to do this as dimension<0, 0, 0>, but you wouldn't want that to be actually dynamic. 
        * Bryce: Potential fix is to use a different, dimension<> like thing to express padding<>
    * Axil: Why not use npos?
        * Bryce: I like this.
        * Eric: std::npos could make this concise. 
    * Bryce: Two branches of my reference implemention, one using 0, one using a magic value (std::experimental::dyn) which is -1.
Extensibility mechanism
    * Carter: Still working on this, we think we are close to having this right.

Specification
    * Eric: Can the datatype by const?
        * Carter: Yes.
        * Bryce: Operates like unique_ptr<> for const-ness
    * Tony: Are there use cases for using reference NOT being add_reference_t<value_type>
    * Axil: Why are we taking integral arguments by template parameter
        * Carter and Bryce: Issues with implicit conversions inhibiting vectorization, especially with indexing operator
        * Bryce: We could change this to size_type and add a note suggesting implementation with a template parameter and SFINAE. Didn't do this for this iteration of the paper.
    * What is the correct size_type? std::size_t?
        * Discussion of whether std::size_t is right.
        * Bryce: "Fixing" std::size_t, while interesting, /is/ outside of the scope of this paper.
        * Axil: Don't let this be implementation defined, bad implementations could make it short.
    * Discussion of span().
        * Eric, Tony: Why do we have span()? What's the use case?
        * Carter: Useful for subarrays.
        * Tony: Dicussed padding by bytes intead of padding by objects.
        * Bryce: I want to pad to by a # of object, not by a # of bytes.
    * Axil: stride()'s argument isn't an 'index', it's a rank. Can we get a separate rank_type for ranks? Even if it's the same as size_type, it would be more expressive.
        * Carter and Bryce: Yes, this is a bug in spec, will fix.
    * Bryce: Just a note, there are some issues with the constructors, for example we don't take property arguments for any constructors, which is wrong. Also, some utility constructors (constructors from nullptr_t, or from a native C array) are missing.
    * Axil and Eric: This is non-owning, why is the destructor non-trivial?
        * Carter and Bryce: This is a bug, will fix.
    * Eric: Should pointer-based constructor be explicit.
    * Tony: We have is_regular, can we SFINAE out stride(), or static_assert?
        * Bryce: Yes.
    * Eric: How about [{0,1,2}] for indexing operator?
        * Carter: Our users would be unhappy.
        * Carter and Bryce: We could do this, or something like [][][] with a DSEL, but the call operator is not complex we believe the best option.
    * Discussion of maximum rank.
        * Carter: We've found we do not have codes that have more than 8 dimensions in practice.
        * Bryce: We have 6.
        * Axil: Why do we need a maximum rank?
        * Johnathon Coe: Feels like a quality of implementation issue, not a specification issue.
        * Bryce: We've discussed handling up to a particular rank, and then going to variadic arguments. It is safe to remove maximum_rank.
        * Tony: That may be observable, you'll need a note explicitly allowing that.
    * Discussion of assignability.
        * Tony: Issues with derived/base type and slicing. Is this an issue here?
        * Axil and Eric: Don't explicitly specialize is_assignable, instead SFINAE.
        * Eric: Design some custom predicate, array_ref_is_assignable. Use magic "Does not participate in overload resolution" language.
        * Bryce: Will overhaul assignability section for next draft.

Subarray interface
    * Axil: Inverse map for regular layout.
    * Axil: array_ref<> conversion that "strips" out a dimension.
        * Carter: Have to be able to figure out if size() == span(), which is a runtime check, so we can't SFINAE this out.
        * Carter: We can do this, if we SFINAE on all dimensions being explicit.
    * Tony: How do we iterate all elements?
        * Carter: You can do a nested for loop.
        * Bryce: Use the subarray interface for iteration. Compilers can be bad at "lifting" common multi-dimensional indexing calculations out of loops. Iterations with the subarray interface can be more performant, and does the same thing that our vectorized kernels do.

Bounds checking
    * Neil and Tony: having bounds checking be part of the type may be a problem. What if we want to turn it on without changing the type.
    * Bryce: Could create a property with runtime state that does bounds checking, enabling you to turn it on or off at runtime.
    * Tony: Why doesn't this have .at()?
        * Bryce: We could have .at().
        * Titus: Not sure this is useful, most common use of .at() I've seen is people being lazy when they have a pointer to a thing.
    * Titus: Not crazy to have it be part of the type.

