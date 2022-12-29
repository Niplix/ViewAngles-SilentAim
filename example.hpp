
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
Vector3 Location;
Location = Fortnites Camera Location, Found in W2S.
  
DWORD_PTR ClosestPawn = NULL;
PlayerController = Read<DWORD_PTR>(Localplayers + 0x30);

if (silentaim && GetAsyncKeyState(VK_RBUTTON))
	{
			uint64_t currentactormesh = Read<uint64_t>(closestPawn + 0x310);
			Vector3 TargetPosition = GetBoneWithRotation(currentactormesh, 66);
			Vector3 TargetPositionToScreen = ProjectWorldToScreen(TargetPosition);

			uintptr_t CurrentWeapon = Read<uintptr_t>(LocalPawn + 0x8d8);
			uintptr_t PlayerCameraManager = Read<uintptr_t>(PlayerController + 0x340);

			if (TargetPositionToScreen.x != 0 || TargetPositionToScreen.y != 0 || TargetPositionToScreen.z != 0)
			{
				if (CurrentWeapon && PlayerCameraManager)
				{
					Vector3 NewRotation = CalculateNewRotation(Location, TargetPosition);

					static float OrginalPitchMin = Read<float>(PlayerCameraManager + 0x3314); // APlayerCameraManager - ViewPitchMin
					static float OrginalPitchMax = Read<float>(PlayerCameraManager + 0x3180); // APlayerCameraManager - ViewPitchMax
          
					Write<float>(CurrentWeapon + 0x1088, NewRotation.x);
					Write<float>(CurrentWeapon + 0x108c, NewRotation.x);

					Write<float>(PlayerCameraManager + 0x3314, NewRotation.y);
					Write<float>(PlayerCameraManager + 0x3180, NewRotation.y);

					Write<float>(PlayerCameraManager + 0x3314, OrginalPitchMin);
					Write<float>(PlayerCameraManager + 0x3180, OrginalPitchMax);

			}
		}
	}
