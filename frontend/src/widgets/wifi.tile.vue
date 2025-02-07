<script setup lang="ts">
import { defineAsyncComponent } from 'vue'
import { useGetWifiState, wifiModes } from '@/data/wifi'
import { RSSItoWifiSignalQualityIndex, wifiSignalQuality } from '@/data/config'

const IconWifi = defineAsyncComponent(() => import('@/assets/icons/wifi.svg'))

const { data: wifiState } = useGetWifiState()
</script>

<template>
  <div class="wifi-tile">
    <span class="wifi-tile__info wifi-tile__info--label">Wi-Fi</span>
    <span class="wifi-tile__info">ip: {{ wifiState?.ip }}</span>
    <span class="wifi-tile__info">mode: {{ wifiModes[wifiState?.mode || '0'] }}</span>
    <div
      class="wifi-tile__icon"
      :class="[wifiSignalQuality[RSSItoWifiSignalQualityIndex(wifiState?.rssi)]]"
    >
      <IconWifi />
    </div>
    <router-link :to="'/wifi'" class="wifi-tile__link" />
  </div>
</template>

<style scoped>
.wifi-tile {
  position: relative;
  display: flex;
  flex-direction: column;
  justify-content: flex-end;
  aspect-ratio: 1;
  height: 10rem;
  width: 10rem;
  padding: 0.6rem;
  color: var(--palette-contrast);
  backdrop-filter: brightness(0.6);
  box-shadow: 0px 10px 15px -3px var(--palette-neutral);
  border-radius: 0.6rem;
  overflow: hidden;
}

.wifi-tile__link {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
}

.wifi-tile__info {
  display: inline-block;
  z-index: 1;
  font-size: large;
}

.wifi-tile__info--label {
  font-size: x-large;
}

.wifi-tile__icon {
  position: absolute;
  top: 0;
  left: 50%;
  width: 100%;
  height: 100%;
  opacity: 0.4;
}

.wifi-tile__icon > :deep(svg) > * {
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
