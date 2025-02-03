<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import type { WifiCredentials } from '@/data/config_api'
import { useGetAp, useSetAp } from '@/data/wifi'

const { data, isLoading } = useGetAp()

const setApCreds = useSetAp()

const handleSave = (creds: WifiCredentials) => {
  setApCreds(creds)
}
</script>

<template>
  <ContentSection title="Access point" subtitle="Edit credentials of self access point">
    <WifiCredentialsForm v-if="!isLoading" :default-values="data" @save="handleSave" />
  </ContentSection>
</template>
