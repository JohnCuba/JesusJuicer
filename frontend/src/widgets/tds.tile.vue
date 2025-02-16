<script lang="ts" setup>
import { useGetTds } from '@/data/tds'
import HomeTile from '@/components/home-tile.vue'
import { defineAsyncComponent } from 'vue'

const IconPpm = defineAsyncComponent(() => import('@/assets/icons/ppm.svg'))

const { data: tdsData } = useGetTds()
</script>

<template>
  <HomeTile>
    <span class="info label">TDS</span>
    <span class="info">{{ tdsData }} ppm</span>
    <div class="icon">
      <IconPpm />
    </div>
  </HomeTile>
</template>

<style lang="css" scoped>
.info {
  display: inline-block;
  z-index: 1;
  font-size: large;
}

.label {
  font-size: x-large;
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
