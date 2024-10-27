#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_LINE(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_SPHERE(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location,25.f, 12, Color, true, 30);
#define DRAW_CAPSULE(Location, Rotation, Color) if (GetWorld()) DrawDebugCapsule(GetWorld(), Location, 10.f, 5.f, Rotation, Color, true, -1.f, 0, 1.f);


#define DRAW_VECTOR(Start, End) if (GetWorld()) \
   { \
      DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f); \
      DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true); \
}

#define DRAW_VECTOR_SINGLEFRAME(Start, End) if (GetWorld()) \
   { \
      DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f); \
      DrawDebugPoint(GetWorld(), Start, 15.f, FColor::Red, false, -1.f); \
}

#define DRAW_SPHERE_SINGLEFRAME(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location,25.f, 12, Color, false, -1.f);
#define DRAWLINE_SINGLEFRAME(Locaation) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f);
#define DRAWPOINT_SINGLEFRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);

