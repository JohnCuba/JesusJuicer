<script setup lang="ts">
import { defineAsyncComponent } from 'vue'

const IconWifi = defineAsyncComponent(() => import('@/assets/icons/wifi.svg'))
</script>

<template>
  <div class="wifi-tile">
    <span class="wifi-tile__label">Wi-Fi</span>
    <div class="wifi-tile__icon">
      <IconWifi />
    </div>
    <router-link :to="'/wifi'" class="wifi-tile__link" />
  </div>
</template>

<style scoped>
.wifi-tile {
  position: relative;
  aspect-ratio: 1;
  height: 10rem;
  width: 10rem;
  border: 1px solid var(--palette-contrast);
  padding: 0.6rem;
  color: var(--palette-contrast);
}

.wifi-tile__link {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
}

.wifi-tile__label {
  z-index: 1;
}

.wifi-tile__icon {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  opacity: 0.4;
}
</style>
