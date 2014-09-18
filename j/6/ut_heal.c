/* j/6/heal.c
**
** This file is in the public domain.
*/
#include "all.h"



/* internals
*/
  static u3_noun
  _heal_as(u3_noun, u3_noun, u3_noun, u3_atom, u3_noun);

  static u3_noun
  _heal_by(
           u3_noun van,
           u3_noun sut,
           u3_noun qog,
           u3_noun ref)
  {
    u3_noun p_sut, q_sut;

    if ( u3_no == u3du(sut) ) {
      return u3_cm_error("heal-name");
    }
    else switch ( u3h(sut) ) {
      default: return u3_cm_error("heal-name");

      case c3__core:
      {
        return u3k(ref);
      }
      case c3__bull: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        if ( u3_yes == u3_cr_sing(u3h(p_sut), u3t(qog)) ) {
          return u3k(ref);
        }
        else {
          u3_noun sub = _heal_by(van, q_sut, qog, ref);
          u3_noun ret;

          ret = u3_cqfu_busk
            (van, sub, u3h(p_sut), u3h(u3t(p_sut)));

          u3z(sub);
          return ret;
        }
      }
      case c3__face: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        if ( u3_yes == u3_cr_sing(p_sut, u3t(qog)) ) {
          return u3_cqf_face(p_sut, ref);
        }
        else return u3_cm_error("heal-name");
      }
      case c3__hold: {
        u3_noun rep = u3_cqfu_repo(van, sut);
        u3_noun ret = _heal_by(van, rep, qog, ref);

        u3z(rep);
        return ret;
      }
      case c3__fork: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        u3_noun dis = _heal_by(van, p_sut, qog, ref);
        u3_noun dat = _heal_by(van, q_sut, qog, ref);
        u3_noun ret = u3_cqf_fork(dis, dat);

        u3z(dis);
        u3z(dat);
        return ret;
      }
    }
  }

  static u3_noun
  _heal_to(
           u3_noun van,
           u3_noun sut,
           u3_noun gil,
           u3_noun qog,
           u3_noun ref,
           u3_atom now,
           u3_atom lat)
  {
    u3_noun p_sut, q_sut;

    if ( u3_no == u3du(sut) ) {
      goto repo;
    }
    else switch ( u3h(sut) ) {
      default: goto repo;

      case c3__atom: return c3__void;

      case c3__bull: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        u3_noun sub = _heal_to(van, q_sut, gil, qog, ref, now, lat);
        u3_noun ret;

        ret = u3_cqfu_busk
          (van, sub, u3h(p_sut), u3h(u3t(p_sut)));

        u3z(sub);
        return ret;
      }
      case c3__cell: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        u3_noun ter, ret;

        if ( 2 == now ) {
          ter = _heal_as(van, p_sut, qog, lat, ref);
          ret = u3_cqf_cell(ter, q_sut);
        } else {
          ter = _heal_as(van, q_sut, qog, lat, ref);
          ret = u3_cqf_cell(p_sut, ter);
        }
        u3z(ter);
        return ret;
      }

      case c3__core: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        if ( 3 != now ) {
          return u3_cm_error("heal-core");
        } else {
          u3_noun ter = _heal_as(van, p_sut, qog, lat, ref);
          u3_noun ret = u3_cqf_core(ter, q_sut);

          u3z(ter);
          return ret;
        }
      }
      case c3__face: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        u3_noun dun = _heal_to(van, q_sut, gil, qog, ref, now, lat);
        u3_noun ret = u3_cqf_face(p_sut, dun);

        u3z(dun);
        return ret;
      }
      case c3__fork: u3_cx_cell(u3t(sut), &p_sut, &q_sut);
      {
        u3_noun dis = _heal_to(van, p_sut, gil, qog, ref, now, lat);
        u3_noun dat = _heal_to(van, q_sut, gil, qog, ref, now, lat);
        u3_noun ret = u3_cqf_fork(dis, dat);

        u3z(dis);
        u3z(dat);
        return ret;
      }

      case c3__hold: {
        if ( (u3_yes == u3_cqdi_has(gil, sut)) ) {
          return c3__void;
        }
        else {
          u3_noun zoc = u3_cqdi_put(gil, sut);
          u3_noun rep = u3_cqfu_repo(van, sut);
          u3_noun ret = _heal_to(van, rep, zoc, qog, ref, now, lat);

          u3z(rep);
          u3z(zoc);

          return ret;
        }
      }
    }

    repo: {
      u3_noun rep = u3_cqfu_repo(van, sut);
      u3_noun ret = _heal_to(van, rep, gil, qog, ref, now, lat);

      u3z(rep);
      return ret;
    }
  }

  static u3_noun
  _heal_as(
           u3_noun van,
           u3_noun sut,
           u3_noun qog,
           u3_atom axe,
           u3_noun ref)
  {
    if ( 1 == axe ) {
      if ( u3_nul == qog ) {
        return u3k(ref);
      } else return _heal_by(van, sut, qog, ref);
    }
    else {
      u3_atom now = u3_cqc_cap(axe);
      u3_atom lat = u3_cqc_mas(axe);
      u3_noun ret = _heal_to(van, sut, u3_nul, qog, ref, now, lat);

      u3z(lat);
      return ret;
    }
  }

/* functions
*/
  u3_noun
  _cqfu_heal(
                        u3_noun van,
                        u3_noun sut,
                        u3_noun qog,
                        u3_noun axe,
                        u3_noun ref)
  {
    return _heal_as(van, sut, qog, axe, ref);
  }

/* boilerplate
*/
  u3_noun
  u3_cwfu_heal(
                       u3_noun cor)
  {
    u3_noun van, sut, qog, axe, ref;

    if ( (u3_no == u3_cr_mean(cor, u3_cv_sam_2, &qog,
                                u3_cv_sam_6, &axe,
                                u3_cv_sam_7, &ref,
                                u3_cv_con, &van,
                                0)) ||
         (u3_no == u3ud(axe)) ||
         (u3_none == (sut = u3_cr_at(u3_cv_sam, van))) )
    {
      return u3_cm_bail(c3__fail);
    } else {
      return _cqfu_heal(van, sut, qog, axe, ref);
    }
  }

  u3_noun
  u3_cqfu_heal(u3_noun van,
                        u3_noun sut,
                        u3_noun qog,
                        u3_noun axe,
                        u3_noun ref)
  {
    return _cqfu_heal(van, sut, qog, axe, ref);
  }