/*
 * Copyright (c) 2015-2015, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_TIME_POINT_SERIALIZE_H
#define SQLPP_TIME_POINT_SERIALIZE_H

#include <date.h>
#include <sqlpp11/result_field.h>
#include <sqlpp11/data_types/time_point/operand.h>
#include <ostream>

namespace sqlpp
{
  template <typename Context, typename Period>
  struct serializer_t<Context, time_point_operand<Period>>
  {
    using _serialize_check = consistent_t;
    using Operand = time_point_operand<Period>;

    static Context& _(const Operand& t, Context& context)
    {
      const auto dp = ::date::floor<::date::days>(t._t);
      const auto time = ::date::make_time(t._t - dp);
      const auto ymd = ::date::year_month_day{dp};
      context << "TIMESTAMP '" << ymd << ' ' << time << "'";
      return context;
    }
  };

  template <typename Db, typename FieldSpec>
  inline std::ostream& operator<<(std::ostream& os, const result_field_t<time_point, Db, FieldSpec>& e)
  {
    if (e.is_null() and not null_is_trivial_value_t<FieldSpec>::value)
    {
      os << "NULL";
    }
    else
    {
      const auto dp = ::date::floor<::date::days>(e.value());
      const auto time = ::date::make_time(e.value() - dp);
      const auto ymd = ::date::year_month_day{dp};
      os << "TIMESTAMP '" << ymd << ' ' << time << "'";
    }
    return os;
  }
}
#endif