<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import NetworkApSection from '@/widgets/network-ap.section.vue'
import SelfApSection from '@/widgets/self-ap.section.vue'
</script>

<template>
  <PageHeader title="/wi-fi" subtitle="home" />
  <div class="flex-auto flex flex-col gap-4 p-4 rounded-xl rounded-b-none bg-base-100">
    <NetworkApSection />
    <SelfApSection />
  </div>
</template>
