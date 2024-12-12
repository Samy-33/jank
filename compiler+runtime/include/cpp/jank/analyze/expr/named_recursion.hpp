#pragma once

#include <jank/runtime/obj/persistent_list.hpp>
#include <jank/analyze/expression_base.hpp>
#include <jank/analyze/expr/recursion_reference.hpp>

namespace jank::analyze::expr
{
  template <typename E>
  struct named_recursion : expression_base
  {
    recursion_reference<E> recursion_ref{};
    runtime::obj::persistent_list_ptr args{};
    native_vector<native_box<E>> arg_exprs;

    void propagate_position(expression_position const pos)
    {
      position = pos;
    }

    runtime::object_ptr to_runtime_data() const
    {
      runtime::object_ptr arg_expr_maps(make_box<runtime::obj::persistent_vector>());
      for(auto const &e : arg_exprs)
      {
        arg_expr_maps = runtime::conj(arg_expr_maps, e->to_runtime_data());
      }

      return runtime::merge(
        static_cast<expression_base const *>(this)->to_runtime_data(),
        runtime::obj::persistent_array_map::create_unique(make_box("__type"),
                                                          make_box("expr::named_recursion"),
                                                          make_box("args"),
                                                          args,
                                                          make_box("arg_exprs"),
                                                          arg_expr_maps));
    }
  };
}