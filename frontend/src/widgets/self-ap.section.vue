<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import type { WifiCredentials } from '@/data/config_api'
import { useGetSelfAp, useSetSelfAp } from '@/data/wifi'

const { data, isLoading } = useGetSelfAp()

const setApCreds = useSetSelfAp()

const handleSave = (creds: WifiCredentials) => {
  setApCreds(creds)
}
</script>

<template>
  <ContentSection title="Self" subtitle="Edit credentials of self access point">
    <WifiCredentialsForm :default-values="data" :is-loading="isLoading" @save="handleSave" />
  </ContentSection>
</template>
