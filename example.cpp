
/*
	Fortnite Silent Aim Externally Example
	https://github.com/DX9Paster
  
        Copyright (c) 2023 DX9Paster
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
 */

bool silentaim = false;
DWORD_PTR ClosestPawn = NULL;
Vector3 Location; // Camera Location

Vector3 CalculateNewRotation(Vector3& zaz, Vector3& daz) {
	Vector3 dalte = zaz - daz;
	Vector3 ongle;
	float hpm = sqrtf(dalte.x * dalte.x + dalte.y * dalte.y);
	ongle.y = atan(dalte.y / dalte.x) * 57.295779513082f;
	ongle.x = (atan(dalte.z / hpm) * 57.295779513082f) * -1.f;
	if (dalte.x >= 0.f) ongle.y += 180.f;
	return ongle;
}

Vector3 GetLoc(Vector3 Loc) {
	Vector3 Location = Vector3(Loc.x, Loc.y, Loc.z);
	return Location;
}

if (silentaim && GetAsyncKeyState(VK_RBUTTON))
{
	uint64_t currentactormesh = Read<uint64_t>(closestPawn + 0x310);
	Vector3 TargetPosition = GetBoneWithRotation(currentactormesh, 66);
	Vector3 TargetPositionToScreen = ProjectWorldToScreen(GetLoc(TargetPosition));

	uintptr_t CurrentWeapon = Read<uintptr_t>(LocalPawn + 0x8d8);
	uintptr_t PlayerCameraManager = Read<uintptr_t>(PlayerController + 0x340);
	
	uintptr_t ViewPitchMin = 0x331c; // APlayerCameraManager - ViewPitchMin - 0x331c
	uintptr_t ViewPitchMax = 0x3320; // APlayerCameraManager - ViewPitchMax - 0x3320
	
        if (TargetPositionToScreen.x != 0 || TargetPositionToScreen.y != 0 || TargetPositionToScreen.z != 0)
	{
		if (CurrentWeapon && PlayerCameraManager)
		{
			Vector3 NewRotation = CalculateNewRotation(Location, TargetPosition);

			static float OrginalPitchMin = Read<float>(PlayerCameraManager + ViewPitchMin);
			static float OrginalPitchMax = Read<float>(PlayerCameraManager + ViewPitchMax);
          
			Write<float>(CurrentWeapon + 0x1088, NewRotation.x); // APlayerCameraManager - ViewYawMin - 0x331c
			Write<float>(CurrentWeapon + 0x108c, NewRotation.x); // AFortWeaponRanged - AimPitchMax - 0x10c4

			Write<float>(PlayerCameraManager + ViewPitchMin, NewRotation.y);
			Write<float>(PlayerCameraManager + ViewPitchMax, NewRotation.y);

			std::this_thread::sleep_for(std::chrono::seconds(5));
			
			Write<float>(PlayerCameraManager + ViewPitchMin, OrginalPitchMin);
			Write<float>(PlayerCameraManager + ViewPitchMax, OrginalPitchMax);
		}
	}
}

// https://fn.dumps.host/?class=APlayerCameraManager 
// https://fn.dumps.host/?class=AFortWeaponRanged
