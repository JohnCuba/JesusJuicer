<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import AccessPointSection from '@/widgets/access-point.section.vue'
import SavedWifisSection from '@/widgets/saved-wifis.section.vue'
</script>

<template>
  <PageHeader with-nav title="Wi-Fi networking" subtitle="Aquaphobic" />
  <SavedWifisSection />
  <AccessPointSection />
</template>
