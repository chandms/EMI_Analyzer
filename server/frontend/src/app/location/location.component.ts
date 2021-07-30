import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

import * as olsource from 'ol/source';
import Map from 'ol/Map';
import TileLayer from 'ol/layer/Tile';
import View from 'ol/View';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { fromLonLat } from 'ol/proj';
import { Vector } from 'ol/layer';

import { DeviceService } from '../service/device.service';


@Component({
  selector: 'location',
  templateUrl: './location.component.html',
  styleUrls: ['./location.component.css']
})
export class LocationComponent implements OnInit {

  constructor(private route: ActivatedRoute, private service: DeviceService) { }

  deviceName: string = '';
  deviceID: number = 0;
  latitude: number = 0;
  longitude: number = 0;
  macAddress: string = '';
  lastUpdate: Date = new Date();
  mapValid: boolean = true;
  
  marker = new Feature({
    geometry: new Point(fromLonLat([this.longitude, this.latitude]))
  });

  layer = new Vector({
    source: new olsource.Vector({features: [this.marker]})
  });

  map!: Map;
  zoom: number = 10;

  ngOnInit(): void {
    this.deviceName = this.route.snapshot.params.deviceName;
    this.service.getDeviceInfo(this.deviceName)
      .subscribe(Response => {
        this.deviceID = Response.device_id
        this.lastUpdate = Response.last_updated;
        this.longitude = Response.longitude;
        this.latitude = Response.latitude;
        if ((this.latitude == null) || (this.longitude == null)) {
         this.mapValid = false;
         this.longitude = 0;
         this.latitude = 0;
        }
        
        let newPosition = fromLonLat([this.longitude, this.latitude]);
        this.map.getView().setCenter(newPosition);
        this.marker.setGeometry(new Point(newPosition));
      })
    
      let position = fromLonLat([this.longitude, this.latitude]);
      this.map = new Map({
        target: 'deviceMap',
        layers: [
          new TileLayer({source: new olsource.OSM()})
        ],
        view: new View({
          center: position,
          zoom: this.zoom
        })
      });
      
      this.map.addLayer(this.layer);    
  }

  changeLocation() {
    let newPosition = fromLonLat([this.longitude, this.latitude]);
    this.map.getView().setCenter(newPosition);
    this.marker.setGeometry(new Point(newPosition));
    this.service.updateLocation(this.deviceID, this.latitude, this.longitude)
      .subscribe(Response => {
        console.log(Response);
      });
  }
}
