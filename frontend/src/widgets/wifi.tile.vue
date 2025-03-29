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
.icon > :deep(svg) > * {
  transition: fill 1s;
}

.very_low > :deep(svg) > *:is(.very_low) {
  fill: var(--palette-danger);
}

.low > :deep(svg) > *:is(.very_low, .low) {
  fill: var(--palette-warning);
}

.good > :deep(svg) > *:is(.very_low, .low, .good) {
  fill: var(--palette-contrast);
}

.very_good > :deep(svg) > *:is(.very_low, .low, .good, .very_good) {
  fill: var(--palette-contrast);
}
</style>
