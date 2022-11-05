#pragma once

#include <unordered_map>

#include <folly/FBVector.h>

#include <jank/runtime/context.hpp>
#include <jank/runtime/obj/symbol.hpp>
#include <jank/option.hpp>

namespace jank::analyze
{
  struct expression;

  struct lifted_var
  {
    runtime::obj::symbol local_name;
    runtime::obj::symbol_ptr var_name;
  };

  struct lifted_constant
  {
    runtime::obj::symbol local_name;
    runtime::object_ptr data;
  };

  struct local_binding
  {
    runtime::obj::symbol_ptr name;
    option<std::reference_wrapper<expression>> value_expr;
  };

  struct local_frame : std::enable_shared_from_this<local_frame>
  {
    enum class frame_type
    {
      root,
      fn,
      let
    };

    local_frame() = delete;
    local_frame(local_frame const &) = default;
    local_frame(local_frame &&) noexcept = default;
    local_frame
    (
      frame_type const &type,
      runtime::context &ctx,
      option<std::shared_ptr<local_frame>> const &p
    );

    local_frame& operator=(local_frame const &rhs);
    local_frame& operator=(local_frame &&rhs);

    /* TODO: Return the binding and all fn frames which were crossed to get it, so they can be
     * updated to register the closure. */
    struct find_result
    {
      local_binding &binding;
      folly::fbvector<std::shared_ptr<local_frame>> crossed_fns;
    };

    /* This is used to find both captures and regular locals, since it's
     * impossible to know which one a sym is without finding it. */
    option<find_result> find_capture(runtime::obj::symbol_ptr const &sym);
    static void register_captures(find_result const &result);

    runtime::obj::symbol_ptr lift_var(runtime::obj::symbol_ptr const &);
    option<std::reference_wrapper<lifted_var const>> find_lifted_var
    (runtime::obj::symbol_ptr const &) const;

    void lift_constant(runtime::object_ptr const &);
    option<std::reference_wrapper<lifted_constant const>> find_lifted_constant
    (runtime::object_ptr const &) const;

    frame_type type;
    option<std::shared_ptr<local_frame>> parent;
    std::unordered_map<runtime::obj::symbol_ptr, local_binding> locals;
    std::unordered_map<runtime::obj::symbol_ptr, local_binding> captures;
    std::unordered_map<runtime::obj::symbol_ptr, lifted_var> lifted_vars;
    std::unordered_map<runtime::object_ptr, lifted_constant> lifted_constants;
    runtime::context &rt_ctx;
  };
  using local_frame_ptr = std::shared_ptr<local_frame>;
}
