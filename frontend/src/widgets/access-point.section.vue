<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
</script>

<template>
  <ContentSection title="Access point" subtitle="Edit credentials of self access point">
    <WifiCredentialsForm />
  </ContentSection>
</template>
