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
    <span class="info">{{ wifiModes[wifiState?.mode || '0'] }}</span>
    <span class="info">{{ wifiState?.ip }}</span>
    <div class="icon" :class="[wifiSignalQuality[RSSItoWifiSignalQualityIndex(wifiState?.rssi)]]">
      <IconWifi />
    </div>
  </HomeTile>
</template>

<style scoped>
.info {
  display: inline-block;
  z-index: 1;
  font-size: large;
}

.icon {
  position: absolute;
  top: 0;
  left: 50%;
  width: 100%;
  height: 100%;
  opacity: 0.4;
}

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
