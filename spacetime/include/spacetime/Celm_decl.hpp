#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/SolverBase_decl.hpp"
#include "spacetime/Selm_decl.hpp"

namespace spacetime
{

/**
 * A compound conservation celm.
 */
class Celm
  : public ElementBase<Celm>
{

public:

    using selm_type = Selm;

    Celm(Field & field, size_t index, bool odd_plane)
      : base_type(&field, field.grid().xptr_celm(index, odd_plane, Grid::CelmPK()))
    {}

    class const_ctor_passkey { const_ctor_passkey(){} friend Field; };

    Celm(Field & field, size_t index, bool odd_plane, const_ctor_passkey)
      : Celm(*const_cast<Field*>(&field), index, odd_plane)
        // TODO: fix the workaround for field constness.
    {}

    sindex_type index() const;

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !on_odd_plane(); }
    bool on_odd_plane() const { return bool((xindex() - (1+Grid::BOUND_COUNT)) & 1); }

    value_type xctr() const { return x(); }

    void move_at(sindex_type offset);

    value_type time_increment() const { return field().time_increment(); }
    value_type dt() const { return field().dt(); }
    value_type hdt() const { return field().hdt(); }
    value_type qdt() const { return field().qdt(); }

    Selm const selm_xn() const { return field().selm<Selm>(index(), on_odd_plane()); }
    Selm       selm_xn()       { return field().selm<Selm>(index(), on_odd_plane()); }
    Selm const selm_xp() const { return field().selm<Selm>(index()+1, on_odd_plane()); }
    Selm       selm_xp()       { return field().selm<Selm>(index()+1, on_odd_plane()); }
    Selm const selm_tn() const { return field().selm<Selm>(index()+on_odd_plane(), !on_odd_plane()); }
    Selm       selm_tn()       { return field().selm<Selm>(index()+on_odd_plane(), !on_odd_plane()); }
    Selm const selm_tp() const { return field().selm<Selm>(index()+on_odd_plane(), !on_odd_plane()); }
    Selm       selm_tp()       { return field().selm<Selm>(index()+on_odd_plane(), !on_odd_plane()); }

    value_type calc_so0(size_t iv) const { return 0.0; }
    value_type calc_so1(size_t iv) const { return 0.0; }

}; /* end class Celm */

template< typename SE >
class CelmBase
  : public Celm
{

public:

    using base_type = Celm;
    using base_type::base_type;
    using selm_type = SE;

    SE const selm_xn() const { return field().template selm<SE>(index(), on_odd_plane()); }
    SE       selm_xn()       { return field().template selm<SE>(index(), on_odd_plane()); }
    SE const selm_xp() const { return field().template selm<SE>(index()+1, on_odd_plane()); }
    SE       selm_xp()       { return field().template selm<SE>(index()+1, on_odd_plane()); }
    SE const selm_tn() const { return field().template selm<SE>(index()+on_odd_plane(), !on_odd_plane()); }
    SE       selm_tn()       { return field().template selm<SE>(index()+on_odd_plane(), !on_odd_plane()); }
    SE const selm_tp() const { return field().template selm<SE>(index()+on_odd_plane(), !on_odd_plane()); }
    SE       selm_tp()       { return field().template selm<SE>(index()+on_odd_plane(), !on_odd_plane()); }

    value_type calc_so0(size_t iv) const;
    value_type calc_so1(size_t iv) const;

}; /* end class CelmBase */

/**
 * Celm index.
 */
sindex_type Celm::index() const
{
    static_assert(0 == (Grid::BOUND_COUNT % 2), "only work with even BOUND_COUNT");
    return (static_cast<sindex_type>(xindex() - 1) >> 1) - 1;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */