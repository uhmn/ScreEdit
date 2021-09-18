#pragma once
std::string unit (int e) { //converts starcraft unit ID to a scratch-craft unit ID
	switch (e) {
	case 7:
		return "ccc";
		break;
	case 98:
		return " ";
		break;
	case 194:
		return "min";
		break;
	case 195:
		return "min";
		break;
	case 196:
		return "min";
		break;
	case 246:
		return "ccc";
		break;
	}
}

int tileout (int tile) {
	if (tile > 31 && tile < 61) {
		return 2;
	}
	if (tile > 63 && tile < 94) {
		return 3;
	}
	return 1;
}