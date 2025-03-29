<script setup lang="ts">
import { defineAsyncComponent } from 'vue'
import {
  useGetWifiState,
  wifiModes,
  RSSItoWifiSignalQualityIndex,
  wifiSignalQuality,
} from '@/data/wifi'
import HomeTile from '@/components/home-tile.vue'

const IconWifi = defineAsyncComponent(() => import('@/assets/icons/wifi.svg'))

const { data: wifiState } = useGetWifiState()
</script>

<template>
  <HomeTile url-path="/wifi" label="Wi-Fi">
    <div class="flex-auto flex flex-col justify-end">
      <span class="font-mono text-xl">{{ wifiModes[wifiState?.mode || '0'] }}</span>
      <span class="font-mono text-xl">{{ wifiState?.ip }}</span>
    </div>
    <IconWifi
      class="absolute top-0 left-1/2 w-full h-full opacity-40 icon"
      :class="[wifiSignalQuality[RSSItoWifiSignalQualityIndex(wifiState?.rssi)]]"
    />
  </HomeTile>
</template>

<style scoped>
.icon > * {
  transition: fill 1s;
}

.very_low > *:is(.very_low) {
  fill: var(--color-error);
}

.low > *:is(.very_low, .low) {
  fill: var(--color-warning);
}

.good > *:is(.very_low, .low, .good) {
  fill: var(--color-success);
}

.very_good > *:is(.very_low, .low, .good, .very_good) {
  fill: var(--color-success);
}
</style>
