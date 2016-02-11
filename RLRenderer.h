#pragma once

#include "RLVector.h"
#include "RLCircle.h"
#include "RLTriangle.h"

using RustyLib::Vector3;
using RustyLib::Geometry::Circle;
using RustyLib::Geometry::Tri;

namespace RustyLib
{
	namespace Graphics
	{
		enum RenderMode {Wireframe, SolidColour, BlendColour, Textured};
		enum ClipMode {None, Basic, CohenSutherland};
		enum LineAlgorithm {DirectScan, DDA, Bresenhams, AndyBresenhams, Wu};
		enum TriAlgorithm {DevMasterNick, FoleyVanDam, MyOwn, Take2};

		class RLRenderer
		{
		private:
			/*******Window Vars**********/
			/********End Window Vars*********/

			/*****Clipping Vars********/
			Vector3 *clipRegionMin;
			Vector3 *clipRegionMax;

			//current Clip Mode - default Basic
			ClipMode currentClipMode;

			//Cohen-Sutherland bit codes
			const char INSIDE;
			const char ABOVE;
			const char BELOW;
			const char LEFT;
			const char RIGHT;
			/*****End Clipping Vars********/

			//Current point size
			unsigned int pointSize;

			//Current line colour
			Vector3* lineColour;
			//Set colour function
			inline void SetAlpha(const float);

			const static float AlphaScalar;// 1 / 255

			//Current method of rasterisation (default: SolidColour)
			RenderMode currentRenderMode;

			//Pixel plotting function
			inline void PlotPixel(int x, int y);
			inline void PlotPixel(int x, int y, int alpha);

			//Clipping methods
			//TriClipping Method
			bool ClipTri(const Tri& tri);
			//PointClipping Method
			bool ClipPoint(const Vector3& p);
			//Cohen-Sutherland Method
			char CohenSutherlandCode(const Vector3& p);

			//Default - Bresenhams
			LineAlgorithm currentLineAlgorithm;
			//Line rasterisation methods
			void DirectScan(const Vector3& p1, const Vector3& p2);
			void DDA(const Vector3& p1, const Vector3& p2);
			void Bresenhams(const Vector3& p1, const Vector3& p2);
			void AndyBresenhams(const Vector3& p1, const Vector3& p2);
			void Wu(const Vector3& p1, const Vector3& p2);
			void PlotScanLine(int x0, int x1, int y);//Basic horizontal span
			void CohenSutherland(const Vector3& p1, const Vector3& p2);

			//Default - MyOwn
			TriAlgorithm currentTriAlgorithm;
			//Triangle raster methods
			void RenderTriWire(const Tri& tri);
			void RenderTriMyOwn(Tri& tri);
			void RenderTriTake2(Tri& tri);
			void RenderTriDevMasterNick(const Tri& tri);//http://www.devmaster.net/forums/showthread.php?t=1884
			void RenderTriFoleyVanDam(const Tri& tri);

			//Text and font attributes
			void* currFont;
			//GLUT_BITMAP_9_BY_15
			//GLUT_BITMAP_8_BY_13
			//GLUT_BITMAP_TIMES_ROMAN_10
			//GLUT_BITMAP_TIMES_ROMAN_24
			//GLUT_BITMAP_HELVETICA_10
			//GLUT_BITMAP_HELVETICA_12
			//GLUT_BITMAP_HELVETICA_18
			
		public:
			RLRenderer(void);
			~RLRenderer(void);

			static RLRenderer& Instance(void );

			/*****Windows methods*******/
			/*****End Windows methods*******/

			/*****Clipping Methods********/
			const ClipMode GetClipMode(void )const;
			void SetClipMode(ClipMode clipMode);
			void SetClippingRegion(const Vector3& min, const Vector3& max);
			void SetClippingRegion(int left, int top, int right, int bottom);
			void RenderClipRegion(void);
			/*****End Clipping Methods********/

			unsigned int GetPointSize(void )const;
			void SetPointSize(unsigned int size);

			Vector3 const& GetLineColour(void )const;
			void SetLineColour(const Vector3& colour);
			void SetLineColour(float r, float g, float b, float a = 1.0f);

			const RenderMode GetRenderMode(void )const;
			void SetRenderMode(RenderMode renderMode);

			const LineAlgorithm GetLineAlgorithm(void )const;
			void SetLineAlgorithm(LineAlgorithm algorithm);

			const TriAlgorithm GetTriAlgorithm(void )const;
			void SetTriAlgorithm(TriAlgorithm algorithm);

			//Public render methods
			//return 0 if it is rendered
			//return 1 if clipped
			int RenderPoint(const Vector3& p);
			int RenderLine(const Vector3& p1, const Vector3& p2);
			int RenderRect(int minX, int minY, int maxX, int maxY);
			int RenderRect(const Vector3& min, const Vector3& max);
			int RenderTri(const Tri& tri);
			int RenderTri(const Vector3* verts);
			int RenderTri(const Vector3& v1, const Vector3& v2, const Vector3& v3);
			int RenderCircle(const Circle& circle);
			int RenderCircle(const Vector3& center, const float radius);

			int RenderText(const std::string& name, const int minX, const int minY);
		};

		typedef RLRenderer Renderer;
	}
}