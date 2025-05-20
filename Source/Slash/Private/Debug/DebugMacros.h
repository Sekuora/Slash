#pragma once
#include "DrawDebugHelpers.h"

// Debug Macros
#define DRAW_SPHERE(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Color, true);
#define DRAW_SPHERE_LIFESPAN(Location, Color, Lifespan) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12, FColor::Color, false, Lifespan);
#define DRAW_SPHERE_SingleFrame(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Color, false, -1.f);


#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);


#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Blue, true);
#define DRAW_POINT_SingleFrame(Location, Color) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Color, false, -1.f);

#define DRAW_BOX(StartLocation, Size) if (GetWorld()) DrawDebugBox(GetWorld(), StartLocation, FVector(Size, Size, Size), FColor::Blue, true, -1.f, 0, 1.f);


// \ allows to continues definition of macro on next line
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, true);\
}

#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if (GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, false, -1.f);\
}