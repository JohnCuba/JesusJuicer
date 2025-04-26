<script setup lang="ts">
import TdsTile from '@/widgets/tds.tile.vue'
import WifiTile from '@/widgets/wifi.tile.vue'
import UpdateTile from '@/widgets/update.tile.vue'
import iconThemeControl from '@/components/icon-theme-control.vue'
</script>

<template>
  <main class="flex-auto p-4 flex flex-col gap-4 bg-base-100">
    <div class="flex-auto grid content-end grid-flow-row-dense grid-cols-2 gap-4">
      <TdsTile class="col-span-2" />
      <WifiTile />
      <UpdateTile />
    </div>
    <div class="flex justify-between align-center">
      <h1 class="flex-none font-sans text-4xl select-none">Jesus Juicer</h1>
      <iconThemeControl />
    </div>
  </main>
</template>
