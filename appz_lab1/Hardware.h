#pragma once

struct HardwareSpec {
	double cpuGHz;
	int    ramGB;
	int    gpuGB;
	int    hddFreeGB;
	HardwareSpec(double cpu, int ram, int gpu, int hdd) : cpuGHz(cpu), ramGB(ram), gpuGB(gpu), hddFreeGB(hdd) {}
};

struct GameRequirements {
	double minCpuGHz;
	int    minRamGB;
	int    minGpuGB;
	int    hddSizeGB;
	GameRequirements(double cpu, int ram, int gpu, int hdd) : minCpuGHz(cpu), minRamGB(ram), minGpuGB(gpu), hddSizeGB(hdd) {}
	bool metBy(const HardwareSpec& hw) const {
		return hw.cpuGHz >= minCpuGHz && hw.ramGB >= minRamGB && hw.gpuGB >= minGpuGB;
	}
	bool hasSpace(const HardwareSpec& hw) const {
		return hw.hddFreeGB >= hddSizeGB;
	}
};
