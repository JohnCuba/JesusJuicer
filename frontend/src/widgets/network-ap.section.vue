<script setup lang="ts">
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
  <div class="flex flex-col gap-1">
    <h3>Edit access point to connect</h3>
    <WifiCredentialsForm
      delitable
      :is-loading="isLoading"
      :default-values="networkCreds"
      @save="handleSave"
      @delete="handleDelete"
    />
  </div>
</template>
