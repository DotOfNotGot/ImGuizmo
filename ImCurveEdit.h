// https://github.com/CedricGuillemet/ImGuizmo
// v 1.89 WIP
//
// The MIT License(MIT)
//
// Copyright(c) 2021 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once
#include <stdint.h>
#include "imgui.h"

#include <span>
#include <set>

struct ImRect;

namespace ImCurveEdit
{
   enum CurveEditFlags
   {
      CurveEditFlags_None = 0,
      CurveEditFlags_MovableCurve = 1 << 0,
      CurveEditFlags_MovablePoints = 1 << 1,
      CurveEditFlags_VerticallyMovablePoints = 1 << 2,
      CurveEditFlags_HorizontallyMovablePoints = 1 << 3,
      CurveEditFlags_OnlyPointMove = 14,
      CurveEditFlags_Zooming = 1 << 4,
      CurveEditFlags_PointMoveWithZoom = 30,
      CurveEditFlags_Panning = 1 << 5,
      CurveEditFlags_ZoomAndPan = 48,
      CurveEditFlags_All = 63
   };

   enum CurveType
   {
      CurveNone,
      CurveDiscrete,
      CurveLinear,
      CurveSmooth,
      CurveBezier,
   };

   struct EditPoint
   {
      int curveIndex;
      int pointIndex;
      bool operator <(const EditPoint& other) const
      {
         if (curveIndex < other.curveIndex)
            return true;
         if (curveIndex > other.curveIndex)
            return false;

         if (pointIndex < other.pointIndex)
            return true;
         return false;
      }
   };

   struct Delegate
   {
      bool focused = false;
      virtual size_t GetCurveCount() = 0;
      virtual bool IsVisible(size_t /*curveIndex*/) { return true; }
      virtual CurveType GetCurveType(size_t /*curveIndex*/) const { return CurveLinear; }
      virtual ImVec2& GetMin() = 0;
      virtual ImVec2& GetMax() = 0;
      virtual size_t GetPointCount(size_t curveIndex) = 0;
      virtual uint32_t GetCurveColor(size_t curveIndex) = 0;
      virtual std::span<ImVec2> GetPoints(size_t curveIndex) = 0;
      virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value) = 0;
      virtual void AddPoint(size_t curveIndex, ImVec2 value) = 0;
      virtual unsigned int GetBackgroundColor() { return 0xFF202020; }
      virtual std::set<ImCurveEdit::EditPoint>& GetSelection() = 0;
      // handle undo/redo thru this functions
      virtual void BeginEdit(int /*index*/) {}
      virtual void EndEdit() {}

      virtual const char* XTooltipName(int curveIndex)
      {
         static const char* name = "X";
         return name;
      }

      virtual const char* YTooltipName(int curveIndex)
      {
         static const char* name = "Y";
         return name;
      }

      virtual ~Delegate() = default;
   };

   int Edit(Delegate& delegate, const ImVec2& size, unsigned int id, CurveEditFlags flags = CurveEditFlags_All, const ImRect* clippingRect = NULL, ImVector<EditPoint>* selectedPoints = NULL);
}
