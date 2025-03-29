<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import { useGetNetworkAP, useSetNetworkAP, useDelNetworkAP } from '@/data/wifi'
import type { WifiCredentials } from '@/data/config_api'

const { data: networkCreds, isLoading } = useGetNetworkAP()
const setNetworkCreds = useSetNetworkAP()
const delNetworkCreds = useDelNetworkAP()

const handleDelete = () => {
  delNetworkCreds()
}

const handleSave = (data: WifiCredentials) => {
  setNetworkCreds(data)
}
</script>

<template>
  <ContentSection title="Network" subtitle="Edit credentials of access point to connect">
    <WifiCredentialsForm
      delitable
      :is-loading="isLoading"
      :default-values="networkCreds"
      @save="handleSave"
      @delete="handleDelete"
    />
  </ContentSection>
</template>
