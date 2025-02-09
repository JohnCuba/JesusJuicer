<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import SelfApSection from '@/widgets/self-ap.section.vue'
import NetworkApSection from '@/widgets/network-ap.section.vue'
</script>

<template>
  <PageHeader with-nav title="Wi-Fi networking" subtitle="Aquaphobic" />
  <NetworkApSection />
  <SelfApSection />
</template>
