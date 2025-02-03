<script setup lang="ts">
import PageHeader from '@/components/page-header.vue'
import ContentSection from '@/components/content-section.vue'
import List from '@/components/list.vue'
import Accordion from '@/components/accordion.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import { useDeleteWifi, useEditWifi, useSaveWifi, useWifiList } from '@/data/wifi'
import { computed, ref } from 'vue'
import type { WifiCredentials } from '@/data/config_api'

const { data } = useWifiList()
const deleteWifi = useDeleteWifi()
const editWifi = useEditWifi()
const saveWifi = useSaveWifi()

const listAdopted = computed(() => {
  const items = data.value?.map((item, index) => ({ slot: 'wifi', key: index.toString(), ...item }))
  items?.push({ slot: 'new', key: data.value?.length.toString() || '0', ssid: '' })
  items?.push({ slot: 'ap', key: ((data.value?.length || 0) + 1).toString() || '1', ssid: '' })
  return items
})

const handleDelete = (index: number) => {
  deleteWifi({ index })
}
const handleEdit = (index: number, data: WifiCredentials) => {
  editWifi({
    index,
    ...data,
  })
}
const handleSave = (data: WifiCredentials) => {
  saveWifi(data)
}
</script>

<template>
  <PageHeader />
  <ContentSection
    title="Saved networks"
    subtitle="Networks are listed in order of connection priority"
  >
    <List :items="listAdopted">
      <template #wifi="{ key, ssid, password, isFocused }">
        <Accordion v-model:open="ref(isFocused).value" class="list__entity">
          <template #summary>
            <p>{{ `${Number(key) + 1}.  ${ssid}` }}</p>
          </template>
          <template #details>
            <WifiCredentialsForm
              delitable
              :default-values="{ ssid, password }"
              @delete="handleDelete(Number(key))"
              @save="(data) => handleEdit(Number(key), data)"
            />
          </template>
        </Accordion>
      </template>
      <template #new="{ isFocused }">
        <Accordion v-model:open="ref(isFocused).value" class="list__entity">
          <template #summary>
            <p>Save new network</p>
          </template>
          <template #details>
            <WifiCredentialsForm @save="handleSave" />
          </template>
        </Accordion>
      </template>
    </List>
  </ContentSection>
  <ContentSection title="Access point" subtitle="Edit credentials of self access point">
    <WifiCredentialsForm />
  </ContentSection>
</template>

<style scoped>
.list__entity {
  padding: 1rem;
}
</style>
