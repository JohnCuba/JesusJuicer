<script setup lang="ts">
import { defineAsyncComponent } from 'vue'
import { version as fsVersion } from '../../package.json'
import HomeTile from '@/components/home-tile.vue'
import { useGetFwVersion } from '@/data/update'

const IconUpdate = defineAsyncComponent(() => import('@/assets/icons/upgrade.svg'))

const { data: fwVersion } = useGetFwVersion()
</script>

<template>
  <HomeTile url-path="/config#update" label="Update">
    <div class="flex flex-auto flex-col justify-end">
      <span v-if="fwVersion === fsVersion" class="font-mono text-xl">{{ fwVersion }}</span>
      <template v-else>
        <span class="font-mono text-xl"> fw: {{ fwVersion }}</span>
        <span class="font-mono text-xl">fs: {{ fsVersion }}</span>
      </template>
    </div>
    <IconUpdate class="absolute top-0 left-1/2 w-full h-full opacity-40" />
  </HomeTile>
</template>
