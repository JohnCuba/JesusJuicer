<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import WifiTile from '@/widgets/wifi.tile.vue'
</script>

<template>
  <PageHeader>
    <WifiTile />
  </PageHeader>
</template>

<style scoped>
.header {
  min-height: 100dvh;
  transition: min-height 0.1s;
}
</style>
