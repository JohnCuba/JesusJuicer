<script setup lang="ts">
import { defineAsyncComponent } from 'vue'
import { version as fsVersion } from '../../package.json'
import HomeTile from '@/components/home-tile.vue'
import { useGetFwVersion } from '@/data/update'

const IconUpdate = defineAsyncComponent(() => import('@/assets/icons/upgrade.svg'))

const { data: fwVersion } = useGetFwVersion()
</script>

<template>
  <HomeTile url-path="/update" label="Update">
    <span v-if="fwVersion === fsVersion">{{ fwVersion }}</span>
    <template v-else>
      <span class="info"> fw: {{ fwVersion }}</span>
      <span class="info">fs: {{ fsVersion }}</span>
    </template>
    <div class="icon">
      <IconUpdate />
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
</style>
