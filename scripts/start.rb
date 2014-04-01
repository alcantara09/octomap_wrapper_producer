require 'orocos'
include Orocos

## Initialize orocos ##
Orocos.initialize

## Execute the task 'message_producer::Task' ##
Orocos.run 'octomap_wrapper_producer::Task' => 'octomap_wrapper_producer' do
  ## Get the task context##
  octomap_wrapper_producer = Orocos.name_service.get 'octomap_wrapper_producer'
  
  ## Start the tasks ##
  octomap_wrapper_producer.start
  
  reader = octomap_wrapper_producer.output.reader
  
  while true
      if msg = reader.read_new
          puts "new msg"
      end
  
      sleep 0.1
  end
end
