#include <jank/runtime/obj/persistent_vector_sequence.hpp>

namespace jank::runtime
{
  obj::persistent_vector_sequence::static_object(obj::persistent_vector_ptr v)
    : vec{ v }
  {
    assert(v->count() > 0);
  }

  obj::persistent_vector_sequence::static_object(obj::persistent_vector_ptr v, size_t i)
    : vec{ v }
    , index{ i }
  {
    assert(v->count() > 0);
  }

  /* behavior::objectable */
  native_bool obj::persistent_vector_sequence::equal(object const &o) const
  {
    return detail::equal(o, vec->data.begin(), vec->data.end());
  }

  void obj::persistent_vector_sequence::to_string(fmt::memory_buffer &buff) const
  {
    return behavior::detail::to_string(
      vec->data.begin()
        + static_cast<decltype(obj::persistent_vector::data)::difference_type>(index),
      vec->data.end(),
      "(",
      ')',
      buff);
  }

  native_persistent_string obj::persistent_vector_sequence::to_string() const
  {
    fmt::memory_buffer buff;
    behavior::detail::to_string(
      vec->data.begin()
        + static_cast<decltype(obj::persistent_vector::data)::difference_type>(index),
      vec->data.end(),
      "(",
      ')',
      buff);
    return { buff.data(), buff.size() };
  }

  native_hash obj::persistent_vector_sequence::to_hash() const
  {
    return hash::ordered(vec->data.begin(), vec->data.end());
  }

  /* behavior::countable */
  size_t obj::persistent_vector_sequence::count() const
  {
    return vec->data.size();
  }

  /* behavior::seqable */
  obj::persistent_vector_sequence_ptr obj::persistent_vector_sequence::seq()
  {
    return this;
  }

  obj::persistent_vector_sequence_ptr obj::persistent_vector_sequence::fresh_seq() const
  {
    return jank::make_box<obj::persistent_vector_sequence>(vec, index);
  }

  /* behavior::sequenceable */
  object_ptr obj::persistent_vector_sequence::first() const
  {
    return vec->data[index];
  }

  obj::persistent_vector_sequence_ptr obj::persistent_vector_sequence::next() const
  {
    auto n(index);
    ++n;

    if(n == vec->data.size())
    {
      return nullptr;
    }

    return jank::make_box<obj::persistent_vector_sequence>(vec, n);
  }

  obj::persistent_vector_sequence_ptr obj::persistent_vector_sequence::next_in_place()
  {
    ++index;

    if(index == vec->data.size())
    {
      return nullptr;
    }

    return this;
  }

  obj::cons_ptr obj::persistent_vector_sequence::conj(object_ptr const head)
  {
    return make_box<obj::cons>(head, this);
  }
}
