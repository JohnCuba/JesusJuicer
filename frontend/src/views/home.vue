<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import WifiTile from '@/widgets/wifi.tile.vue'
import TdsTile from '@/widgets/tds.tile.vue'
import UpdateTile from '@/widgets/update.tile.vue'
</script>

<template>
  <PageHeader>
    <WifiTile />
    <UpdateTile />
    <TdsTile />
  </PageHeader>
</template>

<style scoped>
.header {
  min-height: 100dvh;
  transition: min-height 0.1s;
}

:deep(.header__content) {
  display: flex;
  gap: 1rem;
  flex-flow: wrap-reverse;
  align-content: end;
  justify-content: end;
}
</style>
