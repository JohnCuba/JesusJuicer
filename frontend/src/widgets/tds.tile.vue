<script lang="ts" setup>
import { defineAsyncComponent } from 'vue'
import { useGetTds } from '@/data/tds'
import HomeTile from '@/components/home-tile.vue'

const IconPpm = defineAsyncComponent(() => import('@/assets/icons/ppm.svg'))

const { data: tdsData } = useGetTds()
</script>

<template>
  <HomeTile label="TDS">
    <div class="flex flex-auto flex-col justify-end">
      <span class="font-mono text-xl">{{ tdsData }} ppm</span>
    </div>
    <IconPpm class="absolute top-0 left-1/3 w-full h-full opacity-40" />
  </HomeTile>
</template>
