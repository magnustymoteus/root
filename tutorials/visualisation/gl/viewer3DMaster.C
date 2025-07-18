/// \file
/// \ingroup tutorial_gl
/// Demonstrates 3D viewer architecture TVirtualViewer3D and TBuffer3D in the master frame.
/// Here each shape is described directly in a TBuffer3D
/// class, with identity translation matrix c.f. viewer3DLocal.C
///
/// Our abstract base shape class.
///
/// As we overload TObject::Paint which is called directly from compiled
/// code, this script must also be compiled to work correctly.
///
/// \macro_code
///
/// \author Richard Maunder

#include "TVirtualViewer3D.h"
#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"

#include "TObject.h"
#include "TVirtualPad.h"
#include "TAtt3D.h"

#include <vector>

class Shape : public TObject {
public:
   Shape(Int_t color, Double_t x, Double_t y, Double_t z);
   ~Shape() override{};
   virtual TBuffer3D &GetBuffer3D(UInt_t reqSections) = 0;

protected:
   Double_t fX, fY, fZ; // Origin
   Int_t fColor;

   ClassDefOverride(Shape, 0);
};

ClassImp(Shape);

Shape::Shape(Int_t color, Double_t x, Double_t y, Double_t z) : fX(x), fY(y), fZ(z), fColor(color) {}

class Box : public Shape {
public:
   Box(Int_t color, Double_t x, Double_t y, Double_t z, Double_t dX, Double_t dY, Double_t dZ);
   ~Box() override{};

   TBuffer3D &GetBuffer3D(UInt_t reqSections) override;

private:
   Double_t fDX, fDY, fDZ; // Half lengths

   ClassDefOverride(Box, 0);
};

ClassImp(Box);

Box::Box(Int_t color, Double_t x, Double_t y, Double_t z, Double_t dX, Double_t dY, Double_t dZ)
   : Shape(color, x, y, z), fDX(dX), fDY(dY), fDZ(dZ)
{
}

TBuffer3D &Box::GetBuffer3D(UInt_t reqSections)
{
   static TBuffer3D buffer(TBuffer3DTypes::kGeneric);

   // Complete kCore section - this could be moved to Shape base class
   if (reqSections & TBuffer3D::kCore) {
      buffer.ClearSectionsValid();
      buffer.fID = this;
      buffer.fColor = fColor;   // Color index - see gROOT->GetColor()
      buffer.fTransparency = 0; // Transparency 0 (opaque) - 100 (fully transparent)
      buffer.fLocalFrame = kFALSE;
      buffer.SetLocalMasterIdentity();
      buffer.fReflection = kFALSE;
      buffer.SetSectionsValid(TBuffer3D::kCore);
   }
   // Complete kBoundingBox section
   if (reqSections & TBuffer3D::kBoundingBox) {
      Double_t origin[3] = {fX, fY, fZ};
      Double_t halfLength[3] = {fDX, fDY, fDZ};
      buffer.SetAABoundingBox(origin, halfLength);
      buffer.SetSectionsValid(TBuffer3D::kBoundingBox);
   }
   // No kShapeSpecific section

   // Complete kRawSizes section
   if (reqSections & TBuffer3D::kRawSizes) {
      buffer.SetRawSizes(8, 3 * 8, 12, 3 * 12, 6, 6 * 6);
      buffer.SetSectionsValid(TBuffer3D::kRawSizes);
   }
   // Complete kRaw section
   if (reqSections & TBuffer3D::kRaw) {
      // Points (8)
      // 3 components: x,y,z
      buffer.fPnts[0] = fX - fDX;
      buffer.fPnts[1] = fY - fDY;
      buffer.fPnts[2] = fZ - fDZ; // 0
      buffer.fPnts[3] = fX + fDX;
      buffer.fPnts[4] = fY - fDY;
      buffer.fPnts[5] = fZ - fDZ; // 1
      buffer.fPnts[6] = fX + fDX;
      buffer.fPnts[7] = fY + fDY;
      buffer.fPnts[8] = fZ - fDZ; // 2
      buffer.fPnts[9] = fX - fDX;
      buffer.fPnts[10] = fY + fDY;
      buffer.fPnts[11] = fZ - fDZ; // 3
      buffer.fPnts[12] = fX - fDX;
      buffer.fPnts[13] = fY - fDY;
      buffer.fPnts[14] = fZ + fDZ; // 4
      buffer.fPnts[15] = fX + fDX;
      buffer.fPnts[16] = fY - fDY;
      buffer.fPnts[17] = fZ + fDZ; // 5
      buffer.fPnts[18] = fX + fDX;
      buffer.fPnts[19] = fY + fDY;
      buffer.fPnts[20] = fZ + fDZ; // 6
      buffer.fPnts[21] = fX - fDX;
      buffer.fPnts[22] = fY + fDY;
      buffer.fPnts[23] = fZ + fDZ; // 7

      // Segments (12)
      // 3 components: segment color(ignored), start point index, end point index
      // Indexes reference the above points
      buffer.fSegs[0] = fColor;
      buffer.fSegs[1] = 0;
      buffer.fSegs[2] = 1; // 0
      buffer.fSegs[3] = fColor;
      buffer.fSegs[4] = 1;
      buffer.fSegs[5] = 2; // 1
      buffer.fSegs[6] = fColor;
      buffer.fSegs[7] = 2;
      buffer.fSegs[8] = 3; // 2
      buffer.fSegs[9] = fColor;
      buffer.fSegs[10] = 3;
      buffer.fSegs[11] = 0; // 3
      buffer.fSegs[12] = fColor;
      buffer.fSegs[13] = 4;
      buffer.fSegs[14] = 5; // 4
      buffer.fSegs[15] = fColor;
      buffer.fSegs[16] = 5;
      buffer.fSegs[17] = 6; // 5
      buffer.fSegs[18] = fColor;
      buffer.fSegs[19] = 6;
      buffer.fSegs[20] = 7; // 6
      buffer.fSegs[21] = fColor;
      buffer.fSegs[22] = 7;
      buffer.fSegs[23] = 4; // 7
      buffer.fSegs[24] = fColor;
      buffer.fSegs[25] = 0;
      buffer.fSegs[26] = 4; // 8
      buffer.fSegs[27] = fColor;
      buffer.fSegs[28] = 1;
      buffer.fSegs[29] = 5; // 9
      buffer.fSegs[30] = fColor;
      buffer.fSegs[31] = 2;
      buffer.fSegs[32] = 6; // 10
      buffer.fSegs[33] = fColor;
      buffer.fSegs[34] = 3;
      buffer.fSegs[35] = 7; // 11

      // Polygons (6)
      // 5+ (2+n) components: polygon color (ignored), segment count(n=3+),
      // seg1, seg2 .... segn index
      // Segments indexes refer to the above 12 segments
      // Here n=4 - each polygon defines a rectangle - 4 sides.
      buffer.fPols[0] = fColor;
      buffer.fPols[1] = 4;
      buffer.fPols[2] = 8; // 0
      buffer.fPols[3] = 4;
      buffer.fPols[4] = 9;
      buffer.fPols[5] = 0;
      buffer.fPols[6] = fColor;
      buffer.fPols[7] = 4;
      buffer.fPols[8] = 9; // 1
      buffer.fPols[9] = 5;
      buffer.fPols[10] = 10;
      buffer.fPols[11] = 1;
      buffer.fPols[12] = fColor;
      buffer.fPols[13] = 4;
      buffer.fPols[14] = 10; // 2
      buffer.fPols[15] = 6;
      buffer.fPols[16] = 11;
      buffer.fPols[17] = 2;
      buffer.fPols[18] = fColor;
      buffer.fPols[19] = 4;
      buffer.fPols[20] = 11; // 3
      buffer.fPols[21] = 7;
      buffer.fPols[22] = 8;
      buffer.fPols[23] = 3;
      buffer.fPols[24] = fColor;
      buffer.fPols[25] = 4;
      buffer.fPols[26] = 1; // 4
      buffer.fPols[27] = 2;
      buffer.fPols[28] = 3;
      buffer.fPols[29] = 0;
      buffer.fPols[30] = fColor;
      buffer.fPols[31] = 4;
      buffer.fPols[32] = 7; // 5
      buffer.fPols[33] = 6;
      buffer.fPols[34] = 5;
      buffer.fPols[35] = 4;

      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }

   return buffer;
}

class SBPyramid : public Shape {
public:
   SBPyramid(Int_t color, Double_t d, Double_t y, Double_t z, Double_t dX, Double_t dY, Double_t dZ);
   ~SBPyramid() override{};

   TBuffer3D &GetBuffer3D(UInt_t reqSections) override;

private:
   Double_t fDX, fDY, fDZ; // Base half lengths dX,dY
                           // Pyr. height dZ

   ClassDefOverride(SBPyramid, 0);
};

ClassImp(SBPyramid);

SBPyramid::SBPyramid(Int_t color, Double_t x, Double_t y, Double_t z, Double_t dX, Double_t dY, Double_t dZ)
   : Shape(color, x, y, z), fDX(dX), fDY(dY), fDZ(dZ)
{
}

TBuffer3D &SBPyramid::GetBuffer3D(UInt_t reqSections)
{
   static TBuffer3D buffer(TBuffer3DTypes::kGeneric);

   // Complete kCore section - this could be moved to Shape base class
   if (reqSections & TBuffer3D::kCore) {
      buffer.ClearSectionsValid();
      buffer.fID = this;
      buffer.fColor = fColor;   // Color index - see gROOT->GetColor()
      buffer.fTransparency = 0; // Transparency 0 (opaque) - 100 (fully transparent)
      buffer.fLocalFrame = kFALSE;
      buffer.SetLocalMasterIdentity();
      buffer.fReflection = kFALSE;
      buffer.SetSectionsValid(TBuffer3D::kCore);
   }
   // Complete kBoundingBox section
   if (reqSections & TBuffer3D::kBoundingBox) {
      Double_t halfLength[3] = {fDX, fDY, fDZ / 2.0};
      Double_t origin[3] = {fX, fY, fZ + halfLength[2]};
      buffer.SetAABoundingBox(origin, halfLength);
      buffer.SetSectionsValid(TBuffer3D::kBoundingBox);
   }
   // No kShapeSpecific section

   // Complete kRawSizes section
   if (reqSections & TBuffer3D::kRawSizes) {
      buffer.SetRawSizes(5, 3 * 5, 8, 3 * 8, 5, 6 + 4 * 5);
      buffer.SetSectionsValid(TBuffer3D::kRawSizes);
   }
   // Complete kRaw section
   if (reqSections & TBuffer3D::kRaw) {
      // Points (5)
      // 3 components: x,y,z
      buffer.fPnts[0] = fX - fDX;
      buffer.fPnts[1] = fY - fDY;
      buffer.fPnts[2] = fZ; // 0
      buffer.fPnts[3] = fX + fDX;
      buffer.fPnts[4] = fY - fDY;
      buffer.fPnts[5] = fZ; // 1
      buffer.fPnts[6] = fX + fDX;
      buffer.fPnts[7] = fY + fDY;
      buffer.fPnts[8] = fZ; // 2
      buffer.fPnts[9] = fX - fDX;
      buffer.fPnts[10] = fY + fDY;
      buffer.fPnts[11] = fZ; // 3
      buffer.fPnts[12] = fX;
      buffer.fPnts[13] = fY;
      buffer.fPnts[14] = fZ + fDZ; // 4 (pyr top point)

      // Segments (8)
      // 3 components: segment color(ignored), start point index, end point index
      // Indexes reference the above points

      buffer.fSegs[0] = fColor;
      buffer.fSegs[1] = 0;
      buffer.fSegs[2] = 1; // 0 base
      buffer.fSegs[3] = fColor;
      buffer.fSegs[4] = 1;
      buffer.fSegs[5] = 2; // 1 base
      buffer.fSegs[6] = fColor;
      buffer.fSegs[7] = 2;
      buffer.fSegs[8] = 3; // 2 base
      buffer.fSegs[9] = fColor;
      buffer.fSegs[10] = 3;
      buffer.fSegs[11] = 0; // 3 base
      buffer.fSegs[12] = fColor;
      buffer.fSegs[13] = 0;
      buffer.fSegs[14] = 4; // 4 side
      buffer.fSegs[15] = fColor;
      buffer.fSegs[16] = 1;
      buffer.fSegs[17] = 4; // 5 side
      buffer.fSegs[18] = fColor;
      buffer.fSegs[19] = 2;
      buffer.fSegs[20] = 4; // 6 side
      buffer.fSegs[21] = fColor;
      buffer.fSegs[22] = 3;
      buffer.fSegs[23] = 4; // 7 side

      // Polygons (6)
      // 5+ (2+n) components: polygon color (ignored), segment count(n=3+),
      // seg1, seg2 .... segn index
      // Segments indexes refer to the above 12 segments
      // Here n=4 - each polygon defines a rectangle - 4 sides.
      buffer.fPols[0] = fColor;
      buffer.fPols[1] = 4;
      buffer.fPols[2] = 0; // base
      buffer.fPols[3] = 1;
      buffer.fPols[4] = 2;
      buffer.fPols[5] = 3;

      buffer.fPols[6] = fColor;
      buffer.fPols[7] = 3;
      buffer.fPols[8] = 0; // side 0
      buffer.fPols[9] = 4;
      buffer.fPols[10] = 5;
      buffer.fPols[11] = fColor;
      buffer.fPols[12] = 3;
      buffer.fPols[13] = 1; // side 1
      buffer.fPols[14] = 5;
      buffer.fPols[15] = 6;
      buffer.fPols[16] = fColor;
      buffer.fPols[17] = 3;
      buffer.fPols[18] = 2; // side 2
      buffer.fPols[19] = 6;
      buffer.fPols[20] = 7;
      buffer.fPols[21] = fColor;
      buffer.fPols[22] = 3;
      buffer.fPols[23] = 3; // side 3
      buffer.fPols[24] = 7;
      buffer.fPols[25] = 4;

      buffer.SetSectionsValid(TBuffer3D::kRaw);
   }

   return buffer;
}

class MyGeom : public TObject, public TAtt3D {
public:
   MyGeom();
   ~MyGeom() override;

   void Draw(Option_t *option) override;
   void Paint(Option_t *option) override;

private:
   std::vector<Shape *> fShapes;

   ClassDefOverride(MyGeom, 0);
};

ClassImp(MyGeom);

MyGeom::MyGeom()
{
   // Create our simple geometry - couple of boxes
   // and a square base pyramid
   Shape *aShape;
   aShape = new Box(kRed, 0.0, 0.0, 0.0, 20.0, 20.0, 20.0);
   fShapes.push_back(aShape);
   aShape = new Box(kBlue, 50.0, 100.0, 200.0, 5.0, 10.0, 15.0);
   fShapes.push_back(aShape);
   aShape = new SBPyramid(kGreen, 20.0, 25.0, 45.0, 30.0, 30.0, 90.0);
   fShapes.push_back(aShape);
}

MyGeom::~MyGeom()
{
   // Clear out fShapes
}

void MyGeom::Draw(Option_t *option)
{
   TObject::Draw(option);

   // Ask pad to create 3D viewer of type 'option'
   gPad->GetViewer3D(option);
}

void MyGeom::Paint(Option_t * /*option*/)
{
   TVirtualViewer3D *viewer = gPad->GetViewer3D();

   // If MyGeom derives from TAtt3D then pad will recognise
   // that the object it is asking to paint is 3D, and open/close
   // the scene for us. If not Open/Close are required
   // viewer->BeginScene();

   // We are working in the master frame - so we don't bother
   // to ask the viewer if it prefers local. Viewer's must
   // always support master frame as minimum. c.f. with
   // viewer3DLocal.C
   std::vector<Shape *>::const_iterator ShapeIt = fShapes.begin();
   Shape *shape;
   while (ShapeIt != fShapes.end()) {
      shape = *ShapeIt;

      UInt_t reqSections = TBuffer3D::kCore | TBuffer3D::kBoundingBox | TBuffer3D::kShapeSpecific;
      TBuffer3D &buffer = shape->GetBuffer3D(reqSections);
      reqSections = viewer->AddObject(buffer);

      if (reqSections != TBuffer3D::kNone) {
         shape->GetBuffer3D(reqSections);
         viewer->AddObject(buffer);
      }
      ShapeIt++;
   }
   // Not required as we are TAtt3D subclass
   // viewer->EndScene();
}

void viewer3DMaster()
{
   printf("\n\nviewer3DMaster: This frame demonstates master frame use of 3D viewer architecture.\n");
   printf("Creates two boxes and a square based pyramid, described in master frame.\n\n");

   // macro uses GL features not supported in web graphics
   gROOT->SetWebDisplay("off");

   MyGeom *myGeom = new MyGeom;
   myGeom->Draw("ogl");
}
